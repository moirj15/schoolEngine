#include "VertexBuffer.h"

#include <GL/glew.h>

VertexBuffer::VertexBuffer(
    const f32 *vertecies, const size_t size, std::vector<BufferLayout> layout) :
    m_vertexBufferHandle(0),
    m_layout{std::move(layout)} {
  glGenBuffers(1, &m_vertexBufferHandle);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(f32), vertecies, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &m_vertexBufferHandle);
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferHandle);
}

void VertexBuffer::Unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(const u32 *indecies, const size_t size) :
    m_indexBufferHandle(0), m_count(size) {
  glGenBuffers(1, &m_indexBufferHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, size * sizeof(u32), indecies, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &m_indexBufferHandle);
}

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle);
}

void IndexBuffer::Unbind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::VertexArray() :
    m_vertexArrayHandle(0), m_vertexBuffers{}, m_indexBuffer{} {
  glGenVertexArrays(1, &m_vertexArrayHandle);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &m_vertexArrayHandle);
}

void VertexArray::AddVertexBuffer(VertexBuffer *vertexBuffer) {
  glBindVertexArray(m_vertexArrayHandle);
  vertexBuffer->Bind();
  for (const auto &layout : vertexBuffer->Layout()) {
    glEnableVertexAttribArray(layout.index);
    glVertexAttribPointer(layout.index, layout.elementSize, layout.type,
        GL_FALSE, layout.stride, (void *)0);
  }
  m_vertexBuffers.emplace_back(vertexBuffer);
}

void VertexArray::AddIndexBuffer(IndexBuffer *indexBuffer) {
  glBindVertexArray(m_vertexArrayHandle);
  indexBuffer->Bind();
  m_indexBuffer.reset(indexBuffer);
}

void VertexArray::Bind() const {
  glBindVertexArray(m_vertexArrayHandle);
  m_indexBuffer->Bind();
}

void VertexArray::Unbind() const {
  glBindVertexArray(0);
}
