#pragma once
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

struct PhysicsComponent;
struct MeshComponent;
struct ShaterableComponent;
struct CollidableComponent;

struct ShaterableTuple {
  PhysicsComponent *m_physics;
  MeshComponent *m_mesh;
  ShaterableComponent *m_shaterable;
  CollidableComponent *m_collidable;
};

class ShaterableSystem : public System {
  WorldSystem *m_world;

public:
  ShaterableSystem(WorldSystem *world);

  void Update(f32 t) override;

private:
  std::vector<ShaterableTuple> GetShaterableTuple();
};

} // namespace ecs
