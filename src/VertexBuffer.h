#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>
#include <memory>
#include <string>

#include <GL/glew.h>

#include "common.h"

using GLHandle = u32;


struct BufferLayout {
    std::string name;
    size_t elementSize;
    size_t stride;
    u32 index;
    GLenum type;
};


class VertexBuffer {
    GLHandle m_vertexBufferHandle;
    std::vector<BufferLayout> m_layout;
public:
    VertexBuffer(const f32 const *vertecies, const size_t size, std::vector<BufferLayout> layout);
    ~VertexBuffer();

    void Bind();
    void Unbind();
    inline const std::vector<BufferLayout> &Layout() const { return m_layout; }
};

class IndexBuffer {
    GLHandle m_indexBufferHandle;
    u64 m_count;

public:
    IndexBuffer(const u32 const *indecies, const size_t size);
    ~IndexBuffer();

    void Bind();
    void Unbind();

    u64 Count() const { return m_count; }
};

class VertexArray {
    GLHandle m_vertexArrayHandle;
    std::vector<std::unique_ptr<VertexBuffer>> m_vertexBuffers;
    std::unique_ptr<IndexBuffer> m_indexBuffer;

public:
    VertexArray();
    ~VertexArray();

    void AddVertexBuffer(VertexBuffer *vertexBuffer);
    void AddIndexBuffer(IndexBuffer* indexBuffer);

    void Bind();
    void Unbind();
    u64 IndexCount() const { return m_indexBuffer->Count(); }
};

#endif
