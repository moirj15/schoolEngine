#define GLM_ENABLE_EXPERIMENTAL
#include "skeleton.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

std::vector<SkeletonNode *> SkeletonNode::ToList() {
  if (children.size() <= 0) { return {this}; }
  std::vector<SkeletonNode *> ret;
  ret.emplace_back(this);
  for (auto &childNode : children) {
    auto nodes = childNode->ToList();
    for (auto *node : nodes) { ret.emplace_back(node); }
  }
  return ret;
}

SkeletonNode::BoneList SkeletonNode::ApplyMatricies(const glm::mat4 &parent) {
  if (children.size() <= 0) { return {}; }
  BoneList ret;
  for (const auto &motion : motions) {
    glm::mat4 transform = glm::translate(parent, motion.translations);
    auto rotateX = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.x),
        glm::normalize(glm::vec3{1.0f, 0.0f, 0.0f})));
    auto rotateY = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.y),
        glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f})));
    auto rotateZ = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.z),
        glm::normalize(glm::vec3{0.0f, 0.0f, 1.0f})));
    transform = rotateX * rotateY * rotateZ * transform;
    std::vector<std::pair<glm::vec3, glm::vec3>> bones;
    for (const auto &child : children) {
      bones.emplace_back(transform * glm::vec4{offset, 1.0f},
          transform * glm::vec4{child->offset, 1.0f});
    }
  }
  for (auto &childNode : children) {}
  return {};
}
