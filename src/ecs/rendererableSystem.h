#ifndef RENDERERABLE_SYSTEM_H
#define RENDERERABLE_SYSTEM_H

#include "system.h"

namespace ecs {

class WorldSystem;

class RenderableSystem : public System {
  WorldSystem *m_world;

public:
  RenderableSystem(WorldSystem *world);
  ~RenderableSystem();

  void Update(f32 t) override;
};

} // namespace ecs

#endif
