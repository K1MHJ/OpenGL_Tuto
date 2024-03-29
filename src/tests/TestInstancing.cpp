#include "TestInstancing.hpp"

#include "Renderer.hpp"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test{
  TestInstancing::TestInstancing()
    : m_Proj(glm::ortho(0.0f, 1000.0f, 0.0f, 1000.0f, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      m_Translation(0, 0, 0)
  {
    float positions[] = {
         0.0f,   0.0f, 0.0f, 0.0f, //0
       100.0f,   0.0f, 1.0f, 0.0f, //1
       100.0f, 100.0f, 1.0f, 1.0f, //2
         0.0f, 100.0f, 0.0f, 1.0f  //3
    };
    
    unsigned int indices[] = {
      0,1,2,
      2,3,0
    };

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    
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

    m_Texture = std::make_unique<Texture>("res/textures/logo.png");
    m_Shader->SetUniform1i("u_Texture", 0);
    

    int index = 0;
    float offset = 0.0f;
    for(int y = 0; y < 1000; y += 100)
    {
        for(int x = 0; x < 1000; x += 100)
        {
            glm::vec2 translation;
            translation.x = (float)x;
            translation.y = (float)y;
            translations[index++] = translation;
        }
    }
    for(unsigned int i = 0; i < 100; i++)
    {
      m_Shader->SetUniform2f("offsets[" + std::to_string(i) + "]", translations[i].x, translations[i].y);
    } 
  }
  TestInstancing::~TestInstancing()
  {

  }
  void TestInstancing::OnUpdate(float deltaTime)
  {

  }
  void TestInstancing::OnRender()
  {
    GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    m_Texture->Bind();

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);

      //renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

      m_Shader->Bind();
      m_VAO->Bind();
      m_IndexBuffer->Bind();
      GLCall(glDrawElementsInstanced(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, NULL, 100));
    }
  }
  void TestInstancing::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 1000.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}
