#include "physicsManager.h"

#include "ecs.h"

void PhysicsManager::Simulate(f32 prevTimeStep, f32 currTimeStep) {
  auto physicsComponents = m_componentManager->GetComponents<ECS::Physics>();
  auto transformComponents =
      m_componentManager->GetComponents<ECS::Transform>();
  auto collidableComponents =
      m_componentManager->GetComponents<ECS::Collidable>();
  f32 delta = currTimeStep - prevTimeStep;
  for (size i = 0; i < physicsComponents.size(); i++) {
    auto *transform = transformComponents[i];
    auto *physics = physicsComponents[i];
    auto *collidable = collidableComponents[i];
    if (transform && physics && collidable) { // next line
      // Step 1: calculate forces (friction)
      // frictional force = frictionalCoef * Fn;
      // Fn = m*a = m * 9.8 m/s
      if (glm::length(physics->velocity) > 0.0f) {
        physics->frictionalForce =
            -physics->frictionCoef * (physics->mass * 9.8f) * physics->velocity;
        //            -(physics->frictionCoef * (physics->mass * 9.8f)
        //                * glm::normalize(physics->velocity));

      } else {
        physics->frictionalForce = {0.0f, 0.0f, 0.0f};
      }
      // step 2: integrate position/rotation
      transform->position = transform->position + physics->velocity * delta;
      // Update momentum
      physics->momentum = (physics->mass * physics->velocity)
                          + physics->frictionalForce * delta;

      // step 3: calculate velocities
      if (collidable->collisionDetected) {
        HandleCollision(collidable, transform, physics);
      }
      physics->velocity =
          (physics->momentum / physics->mass) + physics->impulseVelocity;
    }
  }
}

void PhysicsManager::HandleCollision(ECS::Collidable *collidableA,
    ECS::Transform *transformA, ECS::Physics *physicsA) {
  auto collidedEntity = collidableA->collidedEntity;
  auto *transformB =
      m_componentManager->GetComponent<ECS::Transform>(collidedEntity);
  auto *collidableB =
      m_componentManager->GetComponent<ECS::Collidable>(collidedEntity);
  auto *physicsB =
      m_componentManager->GetComponent<ECS::Physics>(collidedEntity);
  glm::vec3 lineOfActionToB = transformB->position - transformA->position;
  glm::vec3 lineOfActionToA = -lineOfActionToB;

  physicsA->impulseVelocity =
      ((physicsA->mass
           * ((physicsB->velocity * glm::normalize(lineOfActionToA))
               - (physicsA->velocity * glm::normalize(lineOfActionToA))))
          / 2.0f)
      * glm::normalize(lineOfActionToA);
  physicsA->impulseVelocity *=
      glm::dot(physicsA->impulseVelocity, lineOfActionToB);
}
