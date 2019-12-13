#include "physicsSystem.h"

#include "ecs.h"

namespace ecs {
PhysicsSystem::PhysicsSystem(WorldSystem *world) {
}
PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(f32 t) {
}
} // namespace ecs

// void PhysicsManager::Simulate(f32 prevTimeStep, f32 currTimeStep) {
//  auto physicsComponents = m_componentManager->GetComponents<ecs::Physics>();
//  auto transformComponents = m_componentManager->GetComponents<ecs::Transform>();
//  for (Size i = 0; i < physicsComponents.size(); i++) {
//    auto *transform = transformComponents[i];
//    auto *physics = physicsComponents[i];
//    if (transform && physics) { // next line
//      transform->position += physics->velocity / 100.0f;
//    }
//  }
//}
