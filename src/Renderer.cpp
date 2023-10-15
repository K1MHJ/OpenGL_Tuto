#include <iostream>
#include "Renderer.hpp"

void GLClearError()
{
  while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
  while(GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}
void Renderer::Clear() const
{
  GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
void Renderer::Draw(const VertexArray& va, const IndexBuffer& id, const Shader& shader)
{
  shader.Bind();
  va.Bind();
  id.Bind();
  //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
  GLCall(glDrawElementsInstanced(GL_LINE_STRIP, 6, GL_UNSIGNED_INT, NULL, 1));
}
