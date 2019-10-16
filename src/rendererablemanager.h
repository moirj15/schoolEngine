#ifndef RENDERERABLEMANAGER_H
#define RENDERERABLEMANAGER_H

#include "ecs.h"

class RendererableManager {
  ECS::ComponentManager *m_componentManager;

public:
  RendererableManager(ECS::ComponentManager *componentManager) :
      m_componentManager{componentManager} {}

  void DrawComponents();
};

#endif // RENDERERABLEMANAGER_H
