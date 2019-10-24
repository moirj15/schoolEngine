#include "collisionManager.h"

f32 top = -7.483;
f32 bottom = 3.483;
f32 left = -1.298;
f32 right = 1.298;

void CheckForWallCollision(ECS::Collidable *collidable, ECS::Transform *transform) {
    auto p = transform->position;
    glm::vec3 topV{transform->position.x, 0.0f, top};
    glm::vec3 bottomV{transform->position.x, 0.0f, bottom};
    glm::vec3 leftV{left, 0.0f, transform->position.z};
    glm::vec3 rightV{right, 0.0f, transform->position.z};
    auto l = glm::length(p - rightV);
    if (glm::length(p - topV) < collidable->radius) {
        transform->position = topV;
        transform->position.z += collidable->radius;
        collidable->collisionWithWall = true;
        collidable->wall = 0;
    }
    else if (glm::length(p - bottomV) < collidable->radius) {
        transform->position = bottomV;
        transform->position.z -= collidable->radius;
        collidable->collisionWithWall = true;
        collidable->wall = 2;
    }
    else if (glm::length(p - leftV) < collidable->radius) {
        transform->position = leftV;
        transform->position.x += collidable->radius;
        collidable->collisionWithWall = true;
        collidable->wall = 3;
    }
    else if (glm::length(p - rightV) < collidable->radius) {
        transform->position = rightV;
        transform->position.x -= collidable->radius;
        collidable->collisionWithWall = true;
        collidable->wall = 1;
    }
}

void CollisionManager::Simulate(f32 prevtimeStep, f32 currTimeStep) {
  auto collidables = m_componentManager->GetComponents<ECS::Collidable>();
  auto transforms = m_componentManager->GetComponents<ECS::Transform>();
  for (size i = 0; i < collidables.size(); i++) {
    auto *collidable = collidables[i];
    auto *transform = transforms[i];
    if (collidable && transform) {
      collidable->collisionDetected = false;
      CheckForCollsion(collidable, transform, i);
      CheckForWallCollision(collidable, transform);
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
        //        c->collisionDetected = true;
        //        c->collidedEntity = id;
      }
    }
  }
}
