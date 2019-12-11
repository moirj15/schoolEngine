#ifndef PARTICLE_H
#define PARTICLE_H

#include "common.h"
#include "renderer/VertexBuffer.h"

#include <glm/mat4x4.hpp>
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

struct Wall {
  glm::vec3 lowerLeft;
  glm::vec3 upperRight;
};

class ParticleEmitter {
  std::vector<Particle> m_particles;
  glm::vec3 m_position;
  glm::vec3 m_velocity;
  glm::vec3 m_emitDirection;
  f32 m_coneBottomRadius;
  f32 m_coneTopRadius;

public:
  ParticleEmitter(const glm::vec3 position, const glm::vec3 velocity, const glm::vec3 emitDirection,
      f32 coneBottomRadius, f32 coneTopRadius, size_t numberOfParticles) :
      m_position(position),
      m_velocity(velocity), m_emitDirection(emitDirection), m_coneBottomRadius(coneBottomRadius),
      m_coneTopRadius(coneTopRadius) {
    SpawnRandomParticles(numberOfParticles);
  }

  void Update(f32 timeStep, const glm::mat4 &transform, const Wall &wall);
  std::unique_ptr<VertexArray> ParticlesToVA();

private:
  void SpawnRandomParticles(size_t num);
  inline glm::vec3 RandomVec3(size_t range) {
    return {(f32)(rand() % range), (f32)(rand() % range), (f32)(rand() % range)};
  }

  inline glm::vec4 RandomVec4(size_t range) {
    return {
        (f32)(rand() % range), (f32)(rand() % range), (f32)(rand() % range), (f32)(rand() % range)};
  }

  Particle SpawnParticle();
};

void UpdateParticles(std::vector<Particle> *particles, f32 t);

#endif
