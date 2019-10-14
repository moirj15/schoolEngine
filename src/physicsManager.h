#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

namespace PhysicsManager {

void Init();

void Deinit();

void Simulate(f32 prevTimeStep, f32 currTimeStep);

} // namespace PhysicsManager

#endif