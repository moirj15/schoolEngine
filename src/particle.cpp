#include "particle.h"

#include <cstdlib>
#include <ctime>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/random.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/transform.hpp>

void ParticleEmitter::Update(f32 timeStep, const glm::mat4 &transform, const Wall &wall) {
  for (size_t i = 0; i < m_particles.size(); i++) {
    if (m_particles[i].lifetime < 0.0f) {
      m_particles[i] = SpawnParticle();
    } else {
      m_particles[i].lifetime -= timeStep;
      auto &pos = m_particles[i].position;
      auto &vel = m_particles[i].velocity;
      f32 distance = 0;
      if (glm::intersectRayPlane(pos, glm::normalize(vel),
              (wall.upperRight - wall.lowerLeft) / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f), distance)) {
        if (distance <= 0.0005f) {
          m_particles[i].velocity = -m_particles[i].velocity;
        }
      }
      // if (pos.y <= wall.lowerLeft.y + 0.5f) {
      //  m_particles[i].velocity = -m_particles[i].velocity;
      //}
      m_particles[i].position +=
          glm::vec3(transform * glm::vec4(m_particles[i].velocity, 1.0f)) * timeStep;
      // m_particles[i].position += m_particles[i].velocity * timeStep;
    }
  }
  m_position += m_velocity * timeStep;
  m_position = transform * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

std::unique_ptr<VertexArray> ParticleEmitter::ParticlesToVA() {
  VertexArray *ret = new VertexArray;
  std::vector<f32> pos;
  std::vector<f32> colors;
  std::vector<u32> index;
  u32 i = 0;
  for (const auto &particle : m_particles) {
    for (Size i = 0; i < 3; i++) {
      pos.push_back(particle.position[i]);
    }
    for (Size i = 0; i < 4; i++) {
      colors.push_back(particle.color[i]);
    }
    index.push_back(i);
    i++;
  }
  ret->AddVertexBuffer(new VertexBuffer(pos.data(), pos.size(), {{"p", 3, 0, 0, GL_FLOAT}}));
  ret->AddVertexBuffer(new VertexBuffer(colors.data(), colors.size(), {{"c", 4, 0, 1, GL_FLOAT}}));
  ret->AddIndexBuffer(new IndexBuffer(index.data(), index.size()));
  return std::unique_ptr<VertexArray>(ret);
}

// Private
void ParticleEmitter::SpawnRandomParticles(size_t num) {
  srand(time(nullptr));
  for (size_t i = 0; i < num; i++) {
    m_particles.emplace_back(SpawnParticle());
  }
}

Particle ParticleEmitter::SpawnParticle() {
  size_t posRange = 4;
  size_t velocityRange = 4;
  size_t colorRange = 256;
  size_t sizeRange = 10;
  size_t lifetimeRange = 50;
  glm::vec3 randBottom(glm::diskRand(m_coneBottomRadius), 0.0f);
  glm::vec3 randTop(glm::diskRand(m_coneTopRadius), 1.0f);
  auto position = randBottom;
  auto velocity = glm::vec3(0.0f, -10.0f, 0.0f) - (randTop - randBottom);
  // velocity = glm::vec3(
  //    glm::rotate(glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
  //    glm::vec4(velocity, 1.0f));
  return {position + m_position, velocity, RandomVec4(colorRange) / 255.0f,
      glm::linearRand(1.0f, (f32)sizeRange), glm::linearRand(1.0f, 2.0f), true};
}
