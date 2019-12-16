#pragma once

#include "../common.h"
#include "system.h"

#include <vector>

namespace ecs {

class WorldSystem;
struct PhysicsComponent;
struct TransformComponent;

struct PhysicsTuple {
  PhysicsComponent *m_physics;
  TransformComponent *m_transform;
};

class PhysicsSystem : public System {
  WorldSystem *m_world;

public:
  PhysicsSystem(WorldSystem *world);
  ~PhysicsSystem();

  void Update(f32 t) override;
private:
  std::vector<PhysicsTuple> GetPhysicsTuples();
};

} // namespace ecs

