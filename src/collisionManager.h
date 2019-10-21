#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "ecs.h"

class CollisionManager {
  ECS::ComponentManager *m_componentManager;

public:
  CollisionManager(ECS::ComponentManager *componentManager) :
    m_componentManager{componentManager} {}

  void Simulate(f32 prevtimeStep, f32 currTimeStep);
};

#endif // COLLISIONMANAGER_H
