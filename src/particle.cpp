#include "particle.h"

#include <cstdlib>
#include <ctime>

static Particle SpawnParticle() {
  srand(time(nullptr));
  size_t posRange = 4;
  size_t velocityRange = 4;
  size_t colorRange = 256;
  size_t sizeRange = 10;
  size_t lifetimeRange = 20;
  return {{(f32)(rand() % posRange), (f32)(rand() % posRange), (f32)(rand() % posRange)},
      {(f32)(rand() % velocityRange), (f32)(rand() % velocityRange), (f32)(rand() % velocityRange)},
      {(f32)(rand() % colorRange) / 256.0f, (f32)(rand() % colorRange) / 256.0f,
          (f32)(rand() % colorRange) / 256.0f, (f32)(rand() % colorRange) / 256.0f},
      (f32)(rand() % sizeRange), (f32)(rand() % lifetimeRange), true};
}

std::vector<Particle> SpawnRandomParticles(size_t num) {
  std::vector<Particle> ret;
  for (size_t i = 0; i < num; i++) {
    ret.emplace_back(SpawnParticle());
  }
  return ret;
}

void UpdateParticles(std::vector<Particle> *particles, f32 t) {
  for (auto &particle : *particles) {
    if (particle.lifetime < 0.0f) {
      particle = SpawnParticle();
    } else {
      particle.lifetime -= t;
      particle.position += particle.velocity * t;
    }
  }
}

std::unique_ptr<VertexArray> ParticlesToVA(const std::vector<Particle> &particles) {
  VertexArray *ret = new VertexArray;
  std::vector<f32> pos;
  std::vector<f32> colors;
  std::vector<u32> index;
  u32 i = 0;
  for (const auto &particle : particles) {
    for (Size i = 0; i < 3; i++) {
      pos.push_back(particle.position[i]);
    }
    for (Size i = 0; i < 4; i++) {
      pos.push_back(particle.color[i]);
    }
    index.push_back(i);
    i++;
  }
  ret->AddVertexBuffer(new VertexBuffer(pos.data(), pos.size(), {{"p", 3, 0, 0, GL_FLOAT}}));
  ret->AddVertexBuffer(new VertexBuffer(colors.data(), colors.size(), {{"c", 4, 0, 1, GL_FLOAT}}));
  ret->AddIndexBuffer(new IndexBuffer(index.data(), index.size()));
  return std::unique_ptr<VertexArray>(ret);
}
