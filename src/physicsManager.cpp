#include "physicsManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "ecs.h"

#include <glm/gtx/string_cast.hpp>

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
        physics->frictionalForce = {0.0f, 0.0f, 0.0f};
        //        physics->frictionalForce = -physics->frictionCoef
        //                                   * (physics->mass * 9.8f)
        //                                   * glm::normalize(physics->velocity);
        //            -(physics->frictionCoef * (physics->mass * 9.8f)
        //                * glm::normalize(physics->velocity));

      } else {
        physics->frictionalForce = {0.0f, 0.0f, 0.0f};
      }
      // step 2: integrate position/rotation
      transform->prevPosition = transform->position;
      transform->position =
          transform->prevPosition + (physics->velocity * delta);
      // Update momentum
      //      if (glm::length(physics->momentum) > 0.0f) {
      //        physics->momentum += physics->frictionalForce * delta;
      //      } else {
      physics->momentum = (physics->mass * physics->velocity)
                          + physics->frictionalForce * delta;
      //      }

      //      printf("%s\n", glm::to_string(physics->momentum).c_str());

      // step 3: calculate velocities
      if (collidable->collisionDetected) {
        HandleCollision(
            prevTimeStep, currTimeStep, collidable, transform, physics);
      } else {
        physics->impulseVelocity = {0.0f, 0.0f, 0.0f};
        physics->velocity =
            (physics->momentum / physics->mass) + physics->impulseVelocity;
      }
      //      if (collidable->collisionDetected) { printf("I HATE YOU\n"); }
    }
  }
}

void PhysicsManager::HandleCollision(f32 prevTimeStep, f32 currTimeStep,
    ECS::Collidable *collidableA, ECS::Transform *transformA,
    ECS::Physics *physicsA) {
  auto collidedEntity = collidableA->collidedEntity;
  auto *transformB =
      m_componentManager->GetComponent<ECS::Transform>(collidedEntity);
  auto *collidableB =
      m_componentManager->GetComponent<ECS::Collidable>(collidedEntity);
  auto *physicsB =
      m_componentManager->GetComponent<ECS::Physics>(collidedEntity);
  collidableA->collisionDetected = false;
  collidableB->collisionDetected = false;

  f32 backupDistanceA =
      glm::length(transformA->position - transformA->prevPosition);
  transformA->position -= backupDistanceA * physicsA->velocity;
  f32 backupDistanceB =
      glm::length(transformB->position - transformB->prevPosition);
  transformB->position -= backupDistanceB * physicsB->velocity;

  glm::vec3 lineOfAction =
      glm::normalize(transformA->position - transformB->position);

  auto vA = physicsA->velocity * glm::dot(physicsA->velocity, lineOfAction);
  auto vB = physicsB->velocity * glm::dot(physicsB->velocity, lineOfAction);
  f32 mass = physicsA->mass;
  f32 restitutionCoef = 0.5;
  glm::vec3 impulse =
      ((mass * (glm::dot(vB, lineOfAction) - glm::dot(vA, lineOfAction)))
          / 2.0f)
      * lineOfAction;
  physicsA->velocity = // impulse;
      (physicsA->velocity + physicsB->velocity) / 2.0f;
  physicsB->velocity = //-impulse;
      -(physicsA->velocity + physicsB->velocity) / 2.0f;
  //  f32 impulse = glm::dot(-(physicsA->velocity - physicsB->velocity)
  //                             * (restitutionCoef + 1.0f),
  //                    lineOfAction)
  //                / (2.0f / mass);
}
