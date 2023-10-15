#pragma once
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include <vector>
#include "Shape.hpp"
#include "imgui/imgui.h"
#include "PositionBuffer.hpp"

namespace Geometry{
  class RectangleCommon
  {
  public:
    static RectangleCommon* Instance(){
      if(pInstance == 0){
        pInstance = new RectangleCommon;
        pInstance->m_IndexBuffer = std::make_unique<IndexBuffer>(RectangleCommon::indices, 6);
        pInstance->m_Shader = std::make_unique<Shader>("res/shaders/Rectangle.shader");
      } 
      return pInstance;
    }
    static const unsigned int indices[];
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
  protected:
    RectangleCommon(){}
    RectangleCommon(const RectangleCommon& rhs){}
    static RectangleCommon* pInstance;
  };

  class Rectangle : public Shape
  {
  public:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<PositionBuffer> m_PositionBuffer;
    std::array<ImVec2, 10000> positions;
    Rectangle(float left, float top, float width, float height)
    {
      float vertexs[] = {
         left,       top, 0.0f, 0.0f, //0
         left+width, top, 1.0f, 0.0f, //1
         left+width, top+height, 1.0f, 1.0f, //2
         left,       top+height, 0.0f, 1.0f  //3
      };

      m_VAO = std::make_unique<VertexArray>();
      m_VertexBuffer = std::make_unique<VertexBuffer>(vertexs, 4 * 4 * sizeof(float));
      m_PositionBuffer = std::make_unique<PositionBuffer>((void*)&positions, 10000 * sizeof(ImVec2));
      
      VertexBufferLayout layout;
      layout.Push<float>(2);
      layout.Push<float>(2);
      m_VAO->AddBuffer(*m_VertexBuffer, layout);
      

    }
    ~Rectangle(){};
    static void SetColor(float R, float G, float B, float alpha){
      RectangleCommon::Instance()->m_Shader->Bind();
      RectangleCommon::Instance()->m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    }
    static void SetMVP(glm::mat4 mvp){
      RectangleCommon::Instance()->m_Shader->Bind();
      RectangleCommon::Instance()->m_Shader->SetUniformMat4f("u_MVP", mvp);
    }
    static void Clear()
    {
      GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }
    void Render()
    {
      RectangleCommon& cmn = *RectangleCommon::Instance();
      cmn.m_Shader->Bind();
      this->m_VAO->Bind();
      cmn.m_IndexBuffer->Bind();
      //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
      GLCall(glDrawElementsInstanced(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, NULL, 1));
    }
  };
}
