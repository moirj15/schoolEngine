#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>
//#include <GLFW/glfw3.h>

#include "common.h"

using GLHandle = u32;

struct BufferLayout {
  std::string name;
  Size elementSize;
  Size stride;
  u32 index;
  GLenum type;
};

class VertexBuffer {
  GLHandle m_vertexBufferHandle;
  std::vector<BufferLayout> m_layout;

public:
  /**
   * @brief VertexBuffer: Constructor.
   * @param vertecies: The list of vertecies.
   * @param size: The size of the vertex list.
   * @param layout: The list of BufferLayout objects that describe the contents of the vertex list.
   */
  VertexBuffer(const f32 *vertecies, const Size size, std::vector<BufferLayout> layout);
  /// Destructor.
  ~VertexBuffer();

  /**
   * @brief Bind: Bind the vertex buffer.
   */
  void Bind() const;
  /**
   * @brief Unbind: Unbind the vertex buffer.
   */
  void Unbind() const;

  /// Getter for the layout.
  inline const std::vector<BufferLayout> &Layout() const { return m_layout; }
};

class IndexBuffer {
  GLHandle m_indexBufferHandle;
  u64 m_count;

public:
  /**
   * @brief IndexBuffer: Constructor.
   * @param indecies: The list of indecies.
   * @param size: The size of the index list.
   */
  IndexBuffer(const u32 *indecies, const Size size);
  /// Destructor.
  ~IndexBuffer();

  /**
   * @brief Bind: Bind the index buffer.
   */
  void Bind() const;

  /**
   * @brief Unbind: Unbind the index buffer.
   */
  void Unbind() const;

  /// Getter for the number of indecies.
  u64 Count() const { return m_count; }
};

class VertexArray {
  GLHandle m_vertexArrayHandle;
  std::vector<std::unique_ptr<VertexBuffer>> m_vertexBuffers;
  std::unique_ptr<IndexBuffer> m_indexBuffer;

public:
  /// Constructor.
  VertexArray();
  /// Destructor.
  ~VertexArray();

  /**
   * @brief AddVertexBuffer: Adds a VertexBuffer to the VertexArray.
   * @param vertexBuffer: The VertexBuffer object that will be added.
   */
  void AddVertexBuffer(VertexBuffer *vertexBuffer);

  /**
   * @brief AddIndexBuffer: Adds an IndexBuffer to the VertexArray.
   * @param indexBuffer: The IndexBuffer object that will be added.
   */
  void AddIndexBuffer(IndexBuffer *indexBuffer);

  /**
   * @brief Bind: Bind the VertexArray.
   */
  void Bind() const;

  /**
   * @brief Unbind: Unbind the VertexArray.
   */
  void Unbind() const;

  /// Getter for the index count.
  u64 IndexCount() const { return m_indexBuffer->Count(); }
};

#endif
