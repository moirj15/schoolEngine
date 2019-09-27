#include "debugdraw.h"
#include "shader.h"
#include "VertexBuffer.h"

#include <vector>

namespace DebugDraw {

struct DebugMesh {
    glm::mat4 transformMat;
    Shader shader;
    VertexArray vertexArray;
};

static std::vector<DebugMesh> debugMeshes;

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

void AddBox(const glm::vec3 &center, const f32 width) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), center);
}

void AddLine(const glm::vec3 &start, const glm::vec3 &end) {

}

}
