#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "common.h"
#include "ecs.h"

class PhysicsManager {
  ECS::ComponentManager *m_componentManager;

public:
  PhysicsManager(ECS::ComponentManager *componenetManager) :
      m_componentManager{componenetManager} {}

  void Simulate(f32 prevTimeStep, f32 currTimeStep);

private:
  void HandleCollision(f32 prevTimeStep, f32 currTimeStep,
      ECS::Collidable *collidable, ECS::Transform *transform,
      ECS::Physics *physics);

}; // namespace PhysicsManager

#endif
