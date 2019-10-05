#include "debugdraw.h"
#include "shader.h"
#include "VertexBuffer.h"
#include "renderer.h"

#include <vector>
#include <array>

namespace DebugDraw {


static std::vector<Renderer::Drawable> s_debugDrawables;
static Shader s_debugShader;

static f32 boxVerts[] = {
    // front
    -1.0, -1.0,  1.0, 1.0,
     1.0, -1.0,  1.0, 1.0,
     1.0,  1.0,  1.0, 1.0,
    -1.0,  1.0,  1.0, 1.0,
    // back
    -1.0, -1.0, -1.0, 1.0,
     1.0, -1.0, -1.0, 1.0,
     1.0,  1.0, -1.0, 1.0,
    -1.0,  1.0, -1.0, 1.0,
};

static u32 boxConn[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
};

void Init() {
    s_debugShader.Compile({"../shaders/debug.vert", "../shaders/debug.frag"});
}

void AddBox(const glm::vec3 &min, const glm::vec3 &max, const glm::vec3 &pos) {
    f32 verts[] = {
        max.x, max.y, max.z, //
        max.x, min.y, max.z, //
        min.x, min.y, max.z, //
        min.x, max.y, max.z, //

        max.x, max.y, min.z, //
        max.x, min.y, min.z, //
        min.x, min.y, min.z, //
        min.x, max.y, min.z, //
    };

    u32 conn[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };
    VertexArray *vertArray = new VertexArray();
    vertArray->AddVertexBuffer(new VertexBuffer(verts, ArraySize(verts),
                                               {{"DBBox", 3, 0, 0, GL_FLOAT}}));
    vertArray->AddIndexBuffer(new IndexBuffer(conn, ArraySize(conn)));
    ShaderData shaderData{"transform", glm::translate(glm::mat4(1.0f), pos)};
    s_debugDrawables.push_back({{}, {shaderData}, vertArray, &s_debugShader});
}

void AddLine(const glm::vec3 &start, const glm::vec3 &end) {

}

const std::vector<Renderer::Drawable> &DebugMeshes() {
    return s_debugDrawables;
}

}
