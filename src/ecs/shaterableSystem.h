#ifndef EXPLODINGBOX_H
#define EXPLODINGBOX_H

#include "../common.h"
#include "system.h"

#include <glm/vec3.hpp>
#include <vector>

namespace ecs {
class WorldSystem;

struct Triangle {
  glm::vec3 p0;
  glm::vec3 p1;
  glm::vec3 p2;
};

class ShaterableSystem : public System {
  WorldSystem *m_world;

public:
  ShaterableSystem(WorldSystem *world);
  ~ShaterableSystem();

  void Update(f32 t) override;
};

} // namespace ecs
#endif // EXPLODINGBOX_H
