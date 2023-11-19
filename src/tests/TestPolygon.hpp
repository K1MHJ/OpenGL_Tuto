#pragma once

#include "Test.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"

#include <GL/glew.h>

namespace test{
  class TestPolygon: public Test
  {
  public:
    TestPolygon();
    ~TestPolygon();

    void OnAttach() override;
    void OnBuild() override;
    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
  private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    glm::vec3 m_TranslationA, m_TranslationB;
    glm::mat4 m_Proj, m_View;
  };
}
