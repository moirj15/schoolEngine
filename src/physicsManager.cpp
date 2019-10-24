#include "physicsManager.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "ecs.h"

#include <glm/gtx/string_cast.hpp>

static f32 e = 1.0;
void HandleWallCollision(ECS::Collidable *collidableA, ECS::Transform *transformA,
                         ECS::Physics *physicsA) {
    collidableA->collisionDetected = false;
  f32 backupDistanceA =
      glm::length(transformA->position - transformA->prevPosition);
  transformA->position -= backupDistanceA * 2.0f * physicsA->velocity ;
  // 0 up, 1 right, 2 down, 3 left
    glm::vec3 lineOfAction;
    if (collidableA->wall == 0) {
        lineOfAction = glm::normalize(glm::vec3{0.0f, 0.0f, -1.0f});
    }
    else if (collidableA->wall == 1) {
        lineOfAction = glm::normalize(glm::vec3{-1.0f, 0.0f, 0.0f});
    }
    else if (collidableA->wall == 2) {
        lineOfAction = glm::normalize(glm::vec3{0.0f, 0.0f, 1.0f});
    }
    else if (collidableA->wall == 3) {
        lineOfAction = glm::normalize(glm::vec3{1.0f, 0.0f, -1.0f});
    }

  f32 mass = physicsA->mass;
  f32 impulse =
      ((mass * ( -glm::dot(physicsA->velocity, lineOfAction)) * (e + 1)));

  physicsA->velocity = physicsA->velocity + ((impulse / mass) * lineOfAction);
  collidableA->collisionWithWall = false;
}

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
                physics->frictionalForce = -physics->frictionCoef
                                           * (physics->mass * 9.8f)
                                           * glm::normalize(physics->velocity);
                    -(physics->frictionCoef * (physics->mass * 9.8f)
                        * glm::normalize(physics->velocity));

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
      if (collidable->collisionWithWall) {
        HandleWallCollision(collidable, transform, physics);
      }
      else if (collidable->collisionDetected) {
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
//  f32 e = 0.5f;
  f32 restitutionCoef = 0.5;
  f32 impulse =
      ((mass * (glm::dot(physicsB->velocity, lineOfAction) - glm::dot(physicsA->velocity, lineOfAction)) * (e + 1)) / 2.0f);

  physicsA->velocity = physicsA->velocity + ((impulse / mass) * lineOfAction);
  physicsB->velocity = physicsB->velocity - ((impulse / mass) * lineOfAction);
}
