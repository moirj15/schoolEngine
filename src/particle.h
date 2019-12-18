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
  /**
   * Constructor
   * @param position: The initial position of the emitter.
   * @param velocity: The velocity of the emitter.
   * @param emitDirection: The emitters direction.
   * @param coneBottomRadius: The radius of the bottom of the cone.
   * @param coneTopRadius: The radius of the top of the cone.
   * @param numberOfParticles: The number of particles to emit.
   */
  ParticleEmitter(const glm::vec3 position, const glm::vec3 velocity, const glm::vec3 emitDirection,
      f32 coneBottomRadius, f32 coneTopRadius, size_t numberOfParticles) :
      m_position(position),
      m_velocity(velocity), m_emitDirection(emitDirection), m_coneBottomRadius(coneBottomRadius),
      m_coneTopRadius(coneTopRadius) {
    SpawnRandomParticles(numberOfParticles);
  }

  /**
   * Updates the particles.
   * @param timeStep: The current time step.
   * @param transform: The current transform.
   * @param wall: A wall to collide with.
   */
  void Update(f32 timeStep, const glm::mat4 &transform, const Wall &wall);

  /// Converts the particles to a vertexArray.
  std::unique_ptr<VertexArray> ParticlesToVA();

private:
  /// Spawns the given number of random particles.
  void SpawnRandomParticles(size_t num);

  /// Makes a random Vec3 in the givne range.
  inline glm::vec3 RandomVec3(size_t range) {
    return {(f32)(rand() % range), (f32)(rand() % range), (f32)(rand() % range)};
  }

  /// Makes a random Vec4 in the givne range.
  inline glm::vec4 RandomVec4(size_t range) {
    return {
        (f32)(rand() % range), (f32)(rand() % range), (f32)(rand() % range), (f32)(rand() % range)};
  }

  /// Spawns a particle
  Particle SpawnParticle();
};

#endif
