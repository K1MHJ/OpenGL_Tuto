#pragma once
class PositionBuffer;
class VertexBuffer;
class VertexBufferLayout;
class VertexArray
{
private:
  unsigned int m_RendererID;
public:
  VertexArray();
  ~VertexArray();
  void AddBuffer(const VertexBuffer& vb, const PositionBuffer& pb, const VertexBufferLayout& layout);
  void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

  void Bind() const;
  void Unbind() const;
};
