#include "collisionManager.h"

void CollisionManager::Simulate(f32 prevtimeStep, f32 currTimeStep) {
  auto collidables = m_componentManager->GetComponents<ECS::Collidable>();
  auto transforms = m_componentManager->GetComponents<ECS::Transform>();
  for (size i = 0; i < collidables.size(); i++) {
    auto *collidable = collidables[i];
    auto *transform = transforms[i];
    if (collidable && transform) {
      collidable->collisionDetected = false;
      CheckForCollsion(collidable, transform, i);
    }
  }
}

void CollisionManager::CheckForCollsion(
    ECS::Collidable *collidable, ECS::Transform *transform, ECS::EntityID id) {
  auto collidables = m_componentManager->GetComponents<ECS::Collidable>();
  auto transforms = m_componentManager->GetComponents<ECS::Transform>();
  for (size i = 0; i < collidables.size(); i++) {
    auto *c = collidables[i];
    auto *t = transforms[i];
    if (c && t && c != collidable && t != transform) {
      f32 distance = glm::length(transform->position - t->position);
      if (distance <= collidable->radius + c->radius) {
        collidable->collisionDetected = true;
        collidable->collidedEntity = (ECS::EntityID)i;
        c->collisionDetected = true;
        c->collidedEntity = id;
      }
    }
  }
}
