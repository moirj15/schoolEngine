#ifndef ECS_H
#define ECS_H

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>
#include <vector>

namespace ECS {

using EntityID = u32;

struct Entity {
  EntityID id;

  Entity(EntityID eid) : id(eid) {}
  virtual ~Entity() {}
};

struct Physics : public Entity {
  glm::vec3 position = {};
  glm::vec3 velocity = {};
  glm::quat angularVelocity = {};
  glm::quat angularMomentum = {};
  glm::vec3 momentum = {};
  f32 mass = 0.0f;

  Physics(EntityID eid) : Entity{eid} {}
};

enum class Type : u32 { Renderable = 1 << 24, Physics = 1 << 25 };

void Init();

void DeInit();

EntityID CreateEntity(Type type);

std::optional<Physics *> GetPhysicsComponent(EntityID id);

std::vector<Physics *> GetPhysics();

} // namespace ECS

#endif
