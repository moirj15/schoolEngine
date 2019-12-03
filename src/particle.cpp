#include "particle.h"

#include <stdlib.h>
#include <time.h>

std::vector<Particle> SpawnRandomParticles(size_t num) {
  std::vector<Particle> ret;
  srand(time(nullptr));
  size_t posRange = 4;
  size_t velocityRange = 4;
  size_t colorRange = 256;
  size_t sizeRange = 10;
  size_t lifetimeRange = 20;
  for (size_t i = 0; i < num; i++) {
    ret.emplace_back({rand() % posRange, rand() % posRange, rand() % posRange},
        {rand() % velocityRange, rand() % velocityRange, rand() % velocityRange},
        {(rand() % colorRange) / 256.0f, (rand() % colorRange) / 256.0f,
            (rand() % colorRange) / 256.0f, (rand() % colorRange) / 256.0f,
            (rand() % colorRange) / 256.0f},
        rand() % sizeRange, rand() % lifetimeRange, true);
  }
  return ret;
}

void UpdateParticles(std::vector<Particle> *particles, f32 t) {
}
