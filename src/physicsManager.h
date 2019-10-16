#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "common.h"

namespace ECS {
class ComponentManager;
}

class PhysicsManager {
  ECS::ComponentManager *m_componentManager;

public:
  PhysicsManager(ECS::ComponentManager *componenetManager) :
      m_componentManager{componenetManager} {}

  void Simulate(f32 prevTimeStep, f32 currTimeStep);

}; // namespace PhysicsManager

#endif