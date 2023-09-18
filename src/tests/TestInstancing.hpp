#pragma once

#include "Test.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"

namespace test{
  class TestInstancing: public Test
  {
  public:
    TestInstancing();
    ~TestInstancing();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<Texture> m_Texture;
    glm::vec3 m_Translation;
    glm::mat4 m_Proj, m_View;

    glm::vec2 translations[100];
  };
}
