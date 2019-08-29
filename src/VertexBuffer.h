#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <memory>

#include "common.h"

using GLHandle = u32;

struct BufferLayout {
    size_t elementSize;
    size_t stride;
};

class VertexBuffer {
    GLHandle m_vertexBufferHandle;
    BufferLayout m_layout;
public:
    VertexBuffer(const f32 const *vertecies, const size_t size);
    ~VertexBuffer();

    void Bind();
    void Unbind();
};

class IndexBuffer {
    GLHandle m_indexBufferHandle;
public:
    IndexBuffer(const u32 const *indecies, const size_t size);
    ~IndexBuffer();

    void Bind();
    void Unbind();
};

class VertexArray {
    std::vector<std::unique_ptr<VertexBuffer>> m_vertexBuffers;
    std::vector<std::unique_ptr<IndexBuffer>> m_indexBuffers;

public:
    VertexArray();
    ~VertexArray();

    void AddVertexBuffer(VertexBuffer *vertexBuffer);
    void AddIndexBuffer(IndexBuffer* indexBuffer);

    void Bind();
    void Unbind();
};

#endif