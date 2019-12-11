#include "physicsManager.h"

#include "ecs.h"

void PhysicsManager::Simulate(f32 prevTimeStep, f32 currTimeStep) {
  auto physicsComponents = m_componentManager->GetComponents<ECS::Physics>();
  auto transformComponents = m_componentManager->GetComponents<ECS::Transform>();
  for (Size i = 0; i < physicsComponents.size(); i++) {
    auto *transform = transformComponents[i];
    auto *physics = physicsComponents[i];
    if (transform && physics) { // next line
      transform->position += physics->velocity / 100.0f;
    }
  }
}
