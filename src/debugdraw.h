#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>
class VertexArray;
class Shader;

namespace DebugDraw {

struct DebugMesh {
    glm::mat4 transformMat;
    Shader *shader;
    std::unique_ptr<VertexArray> vertexArray;

    DebugMesh(glm::mat4 &t, Shader *s, VertexArray *v) :
        transformMat(std::move(t)), shader(s), vertexArray(v) {}
};

/**
 * Initilize the DebugDraw Module.
 */
void Init();

/**
 * Adds a bux to the debug module.
 * @param min: The minimum vertex of the box.
 * @param max: The maximum vertex of the box.
 * @param pos: The position of the center of the box.
 */
void AddBox(const glm::vec3 &min, const glm::vec3 &max, const glm::vec3 &pos);

void AddLine(const glm::vec3 &start, const glm::vec3 &end);

/**
 * Getter for the DebugMeshes from the DebugDraw module.
 * @return: The DebugMeshes
 */
const std::vector<DebugMesh> &DebugMeshes();
}

#endif // DEBUGDRAW_H
