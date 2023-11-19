#include "TestTile.hpp"

#include "Renderer.hpp"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <stdio.h>

namespace test{
  TestTile::TestTile()
    : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      m_TranslationA(200, 200, 0), m_TranslationB(400,200,0)
  {
    OnAttach();
    OnBuild();
  }
  void TestTile::OnBuild()
  {
    float positions[] = {
      -50.0f, -50.0f, 0.0f, 0.0f, //2
       50.0f, -50.0f, 10.0f, 0.0f, //1
       50.0f,  50.0f, 10.0f, 10.0f, //0
      -50.0f,  50.0f, 0.0f, 10.0f  //3
    };
    
    unsigned int indices[] = {
      0,1,2,
      2,3,0
    };
    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);
    m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    m_Shader->SetUniform1i("u_Texture", 0);

    m_Texture = std::make_unique<Texture>("res/textures/logo.png");
    m_Texture->Unbind();

    GLCall(glActiveTexture(GL_TEXTURE0 + 0));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RenderedTexture));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //GLCall(glBindTexture(GL_TEXTURE_2D, 0));
  }
  TestTile::~TestTile()
  {
    glDeleteTextures(1, &m_RenderedTexture);
  }
  void TestTile::OnUpdate(float deltaTime)
  {

  }
  void TestTile::OnAttach()
  {
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    glGenFramebuffers(1, &m_Framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glGenTextures(1, &m_RenderedTexture);
    printf("m_RenderedTexture : %d \n", m_RenderedTexture);
    glBindTexture(GL_TEXTURE_2D, m_RenderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenRenderbuffers(1, &m_Depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_Depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 32, 32);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Depthbuffer);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_RenderedTexture, 0);
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer);
    glViewport(0,0,32,32);
    float vertices[] = {
        -16, 0, 0, // Start point
         16, 0, 0  // End point
    }; 
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    // Fragment shader
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
        "}\n\0";
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glClearColor(0.4f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 2); // Draw the line

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glBindTexture(GL_TEXTURE_2D, 0);
  }
  void TestTile::OnRender()
  {
    Renderer renderer;
    //m_Texture->Bind();
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
    }
  }
  void TestTile::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}
