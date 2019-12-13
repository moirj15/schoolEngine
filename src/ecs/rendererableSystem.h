#pragma once

#include "system.h"

#include <vector>
namespace ecs {

class WorldSystem;
struct RenderableComponent;
struct TransformComponent;
struct MeshComponent;

struct RenderableTuple {
  RenderableComponent *m_renderable;
  TransformComponent *m_transform;
  MeshComponent *m_mesh;
};

class RenderableSystem : public System {
  WorldSystem *m_world;

public:
  RenderableSystem(WorldSystem *world);
  ~RenderableSystem();

  void Update(f32 t) override;

private:
  std::vector<RenderableTuple> GetRenderableTuple();
};

} // namespace ecs

