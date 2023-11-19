#include "TestPolygon.hpp"

#include "Renderer.hpp"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <stdio.h>

namespace test{
  TestPolygon::TestPolygon()
    : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
      m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
      m_TranslationA(200, 200, 0), m_TranslationB(400,200,0)
  {
    OnAttach();
    OnBuild();
  }
  void TestPolygon::OnBuild()
  {
    const int patternLength = 16;
    unsigned char stipplePattern[patternLength] = { 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0, 255, 0 }; // Example pattern

    unsigned int stippleTexture;
    glGenTextures(1, &stippleTexture);
    glBindTexture(GL_TEXTURE_1D, stippleTexture);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RED, patternLength, 0, GL_RED, GL_UNSIGNED_BYTE, stipplePattern);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float positions[] = {
      -50.0f, -50.0f,
       50.0f, -50.0f,
       70.0f,   0.0f,
       50.0f,  50.0f,
      -50.0f,  50.0f,
      -70.0f,   0.0f
    };
    m_VAO = std::make_unique<VertexArray>();
    m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 6 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);

    m_Shader = std::make_unique<Shader>("res/shaders/Polygon.shader");
    m_Shader->Bind();
    m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
  }
  TestPolygon::~TestPolygon()
  {
  }
  void TestPolygon::OnUpdate(float deltaTime)
  {

  }
  void TestPolygon::OnAttach()
  {
  }
  void TestPolygon::OnRender()
  {
    Renderer renderer;
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      m_VAO->Bind();
      m_Shader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0xF0F0);
      glLineWidth(3);
      glDrawArrays(GL_LINE_LOOP, 0, 6);
      glDisable(GL_LINE_STIPPLE);

    }
    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
      glm::mat4 mvp = m_Proj * m_View * model;
      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_MVP", mvp);
      m_VAO->Bind();
      m_Shader->SetUniform4f("u_Color", 0.0f, 1.0f, 0.0f, 1.0f);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
      glEnable(GL_LINE_SMOOTH);
      m_Shader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
      glDrawArrays(GL_LINE_LOOP, 0, 6);
      glDisable(GL_LINE_SMOOTH);
    }
  }
  void TestPolygon::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
    ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}
