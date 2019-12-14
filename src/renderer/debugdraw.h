#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "shader.h"

class VertexArray;
namespace renderer { struct Drawable; }

namespace DebugDraw {

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
const std::vector<renderer::Drawable> &DebugMeshes();
}

#endif // DEBUGDRAW_H
