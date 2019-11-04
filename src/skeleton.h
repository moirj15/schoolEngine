#ifndef SKELETON_H
#define SKELETON_H

#include "common.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <vector>

constexpr u32 X_POS = 1 << 0;
constexpr u32 Y_POS = 1 << 1;
constexpr u32 Z_POS = 1 << 2;
constexpr u32 X_ROT = 1 << 3;
constexpr u32 Y_ROT = 1 << 4;
constexpr u32 Z_ROT = 1 << 5;

struct SkeletonNode {
  u32 allowedMotions;
  glm::vec3 offset;
  std::string name;
  std::vector<std::unique_ptr<SkeletonNode>> m_children;
  SkeletonNode(const std::string &n) : name{n} {}
};

class Skeleton {

public:
};

#endif // SKELETON_H
