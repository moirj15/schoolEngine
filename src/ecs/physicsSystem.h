#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "../common.h"
#include "system.h"

namespace ecs {

class WorldSystem;

class PhysicsSystem : public System {
  WorldSystem *m_world;

public:
  PhysicsSystem(WorldSystem *world);
  ~PhysicsSystem();

  void Update(f32 t) override;
};

} // namespace ecs

#endif