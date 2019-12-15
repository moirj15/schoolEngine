#pragma once
#include "../common.h"
#include "system.h"

#include <glm/vec3.hpp>
#include <vector>

namespace ecs {
class WorldSystem;

struct PhysicsComponent;
struct MeshComponent;
struct ShaterableComponent;
struct CollidableComponent;
struct DECLComponent;

struct ShaterableTuple {
  u64 m_id;
  PhysicsComponent *m_physics;
  MeshComponent *m_mesh;
  ShaterableComponent *m_shaterable;
  CollidableComponent *m_collidable;
  DECLComponent *m_DECL;
};

class ShaterableSystem : public System {
  WorldSystem *m_world;

public:
  ShaterableSystem(WorldSystem *world);

  void Update(f32 t) override;

private:
  std::vector<ShaterableTuple> GetShaterableTuple();
  void DecomposeShaterable(ShaterableTuple *tuple);
};

} // namespace ecs
