#ifndef SYSTEM_H
#define SYSTEM_H

#include "../common.h"

namespace ecs {

struct System {
  virtual ~System() {}
  virtual void Update(f32 t) = 0;
};

} // namespace ecs

#endif
