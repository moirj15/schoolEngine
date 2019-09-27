#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <glm/glm.hpp>

namespace DebugDraw {

void AddBox(const glm::vec3 &center);

void AddLine(const glm::vec3 &start, const glm::vec3 &end);

}

#endif // DEBUGDRAW_H
