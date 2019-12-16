#include "physicsSystem.h"

#include "ecs.h"
#include "components.h"

namespace ecs {
PhysicsSystem::PhysicsSystem(WorldSystem *world) : m_world(world) {
}
PhysicsSystem::~PhysicsSystem() {
}

void PhysicsSystem::Update(f32 t) {
  for (auto &tuple : GetPhysicsTuples()) {
    auto *transform = tuple.m_transform;
    auto *physics = tuple.m_physics;
    transform->m_position += physics->m_velocity * t;
  }
}

std::vector<PhysicsTuple> PhysicsSystem::GetPhysicsTuples() {
  std::vector<PhysicsTuple> ret;
  auto ids = m_world->EntityIDsWithType(TupleType::PhysicsTuple);
  for (auto id : ids) {
    if (m_world->IsValid(id)) {
      auto [physics, transform] =
      m_world->GetTuple<PhysicsComponent *, TransformComponent *>(id);
      ret.push_back({physics, transform});
    }
  }

  return ret;
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
