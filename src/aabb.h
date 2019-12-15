#pragma once

#include "common.h"

#include <glm/glm.hpp>
#include <array>
#include <vector>

struct Ray {
  glm::vec3 origin;
  glm::vec3 direction;

  glm::vec3 inverseDir;
  s32 sign[3];

  Ray(const glm::vec3 &orig, const glm::vec3 &dir) : origin(orig), direction(dir) {
    inverseDir = 1.0f / dir;
    sign[0] = inverseDir.x < 0;
    sign[1] = inverseDir.y < 0;
    sign[2] = inverseDir.z < 0;
  }
};

struct AABB {
  glm::vec3 m_bounds[2];
  std::array<u32, 3> m_triangleIndexes;

public:
  AABB() = default;
  AABB(const glm::vec3 &a, const glm::vec3 &b) : m_bounds{a, b} {}

  bool IntersectRay(const Ray &ray);
  bool IntersectAABB(const AABB &aabb);
};

struct AABBTree {
  AABBTree *m_left;
  AABBTree *m_right;
  AABB m_node;
};

AABBTree *ConstructAABBTree(const std::vector<f32> &verts, const std::vector<u32> &connections);
void DeleteAABBTree(AABBTree *tree);