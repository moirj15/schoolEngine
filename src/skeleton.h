#ifndef SKELETON_H
#define SKELETON_H

#include "common.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

constexpr u32 X_POS = 1 << 0;
constexpr u32 Y_POS = 1 << 1;
constexpr u32 Z_POS = 1 << 2;
constexpr u32 Z_ROT = 1 << 3;
constexpr u32 X_ROT = 1 << 4;
constexpr u32 Y_ROT = 1 << 5;

struct NodeMotion {
  glm::vec3 translations = {};
  glm::vec3 rotations = {};
};

using BoneList = std::vector<std::pair<glm::vec3, glm::vec3>>;
struct SkeletonNode {
  u32 allowedMotions = 0;
  glm::vec3 offset = {};
  std::string name = {};
  std::vector<NodeMotion> motions;
  std::vector<std::unique_ptr<SkeletonNode>> children = {};
  SkeletonNode(const std::string &n) : name{n} {}

  std::vector<SkeletonNode *> ToList();

  std::vector<glm::mat4> Transformations(
      size i, const glm::mat4 &parent = glm::mat4{1.0f});

  BoneList Bones();

private:
  BoneList ApplyMatriciesRecursive(
      size motionIndex, const glm::mat4 &parent = glm::mat4(1.0f));
};

// class Skeleton {
//  std::unique_ptr<SkeletonNode> m_skeletonTree;
//  size m_motionCount;
//};

#endif // SKELETON_H
