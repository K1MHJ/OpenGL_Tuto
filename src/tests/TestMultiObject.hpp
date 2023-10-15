#pragma once

#include "Test.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
//#include "Geometry.hpp"
#include <vector>
#include "PositionBuffer.hpp"

namespace test{
  class TestMultiObject: public Test
  {
  public:
    TestMultiObject();
    ~TestMultiObject();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;
  private:
    //std::vector<Geometry::Rectangle*> vecRect;
    glm::vec3 m_TranslationA;
    glm::mat4 m_Proj, m_View;


    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<PositionBuffer> m_PositionBuffer;
    glm::vec2 positions[10000];
    unsigned int vao;
  };
}
