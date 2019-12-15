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
  /*
   * Constructor
   *
   * @param world: The world the system belongs to.
   */
  RenderableSystem(WorldSystem *world);


  void Update(f32 t) override;

private:
  /**
   * Gets the components that make up a renderableTuple from the world.
   * @return: The list of RenderableTuples.
   */
  std::vector<RenderableTuple> GetRenderableTuple();
};

} // namespace ecs

