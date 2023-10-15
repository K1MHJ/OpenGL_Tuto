#include "TestMultiObject.hpp"

#include "Renderer.hpp"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "PositionBuffer.hpp"


namespace test{

  TestMultiObject::TestMultiObject()
    : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      m_TranslationA(0, 0, 0)
  {
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_Shader = std::make_unique<Shader>("res/shaders/Rectangle.shader");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    unsigned int indices[] = 
    {
      0,1,2,
      0,3,2
    };
    float left = 0.0f;
    float top = 0.0f;
    float width = 5.0f;
    float height = 5.0f;

    glm::vec2 vertices[4];
    vertices[0] = glm::vec2(-width/2, -height/2);
    vertices[1] = glm::vec2(width/2,-height/2);
    vertices[2] = glm::vec2(width/2, height/2);
    vertices[3] = glm::vec2(-width/2,height/2);

    int index = 0;
    for(int y = 0; y < 1000; y += 10)
    {
        for(int x = 0; x < 1000; x += 10)
        {
            glm::vec2 translation;
            translation.x = (float)x;
            translation.y = (float)y;
            positions[index++] = translation;
        }
    }

    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * sizeof(glm::vec2));
    m_VertexBuffer->Unbind();
    m_PositionBuffer = std::make_unique<PositionBuffer>((void*)&positions, 10000 * sizeof(glm::vec2));
    m_PositionBuffer->Unbind(); 
      
    m_VAO = std::make_unique<VertexArray>();
    m_VAO->Bind();

    glEnableVertexAttribArray(0);
    m_VertexBuffer->Bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );
    
    glEnableVertexAttribArray(1);
    m_PositionBuffer->Bind();
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    
    glBindVertexArray(0);
  }
  TestMultiObject::~TestMultiObject()
  {

  }
  void TestMultiObject::OnUpdate(float deltaTime)
  {

  }
  void TestMultiObject::OnRender()
  {
    GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
    glm::mat4 mvp = m_Proj * m_View * model;

    {
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      m_Shader->Bind();

      m_VAO->Bind();
      m_IndexBuffer->Bind();
      GLCall(glDrawElementsInstanced(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, NULL, 10000));
    }
  }
  void TestMultiObject::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}
