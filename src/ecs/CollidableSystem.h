//
// Created by Jimmy on 12/15/2019.
//

#pragma once
#include "../common.h"
#include "system.h"
#include <glm/vec3.hpp>

#include <vector>
namespace ecs {
class WorldSystem;

struct CollidableComponent;
struct DECLComponent;
struct TransformComponent;
struct MeshComponent;

struct CollidableTuple {
  CollidableComponent *m_collidable;
  DECLComponent *m_DECL;
  TransformComponent *m_transform;
  MeshComponent *m_mesh;
};

class CollidableSystem : public System {
  WorldSystem *m_world;

public:
  CollidableSystem(WorldSystem *world);

  void Update(f32 t) override;

private:
  std::vector<CollidableTuple> GetCollidableTuples();
  void UpdateDecl(CollidableTuple *tuple, const glm::vec3 &position);
};

} // namespace ecs
