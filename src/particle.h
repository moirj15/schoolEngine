#ifndef PARTICLE_H
#define PARTICLE_H

#include "common.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct Particle {
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec4 color;
  f32 size;
  f32 lifetime;
  bool alive;
};

std::vector<Particle> SpawnRandomParticles(size_t num);

void UpdateParticles(std::vector<Particle> *particles, f32 t);

#endif
