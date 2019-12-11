#ifndef EXPLODINGBOX_H
#define EXPLODINGBOX_H

#include "../common.h"

#include <glm/vec3.hpp>
#include <vector>

namespace ECS {
struct Mesh;
class ComponentManager;
} // namespace ECS

struct Triangle {
  glm::vec3 p0;
  glm::vec3 p1;
  glm::vec3 p2;
};

class ShaterableManager {
  ECS::ComponentManager *m_componentManager;

public:
  ShaterableManager(ECS::ComponentManager *componentManager) :
      m_componentManager{componentManager} {}

  void Simulate(f32 prevTimeStep, f32 currTimeStep);

private:
  std::vector<Triangle> ProcessTriangles(ECS::Mesh *mesh);

  void AddTrianglesToECS(const std::vector<Triangle> &triangles);
};

#endif // EXPLODINGBOX_H
