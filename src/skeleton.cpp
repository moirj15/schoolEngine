#define GLM_ENABLE_EXPERIMENTAL
#include "skeleton.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

std::vector<SkeletonNode *> SkeletonNode::ToList() {
  if (children.size() <= 0) {
    return {this};
  }
  std::vector<SkeletonNode *> ret;
  ret.emplace_back(this);
  for (auto &childNode : children) {
    auto nodes = childNode->ToList();
    for (auto *node : nodes) {
      ret.emplace_back(node);
    }
  }
  return ret;
}

static glm::mat4 CalculateTransform(const NodeMotion &motion,
    const glm::mat4 &parent, const glm::vec3 &offset) {

  glm::mat4 transform = glm::translate(parent, motion.translations);
  glm::mat4 offsetTransform = glm::translate(parent, offset);

  auto rotateX = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.x),
      glm::normalize(glm::vec3{1.0f, 0.0f, 0.0f})));

  auto rotateY = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.y),
      glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f})));

  auto rotateZ = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.z),
      glm::normalize(glm::vec3{0.0f, 0.0f, 1.0f})));

  transform = rotateX * rotateY * rotateZ * transform * offsetTransform;
  return transform;
}

std::vector<glm::mat4> SkeletonNode::Transformations(
    size i, const glm::mat4 &parent) {
  if (children.size() <= 0) {
    printf("OUT\n");
    return {glm::translate(glm::mat4{1.0f}, offset)};
  }
  auto transform = CalculateTransform(motions[i], parent, offset);
  std::vector<glm::mat4> ret;
  ret.emplace_back(transform);
  printf("IN\n");
  for (auto &child : children) {
    auto childTransforms = child->Transformations(i, transform);
    for (auto &childTransform : childTransforms) {
      ret.emplace_back(childTransform);
    }
  }
  return ret;
}

// BoneList SkeletonNode::ApplyMatriciesRecursive(
//    size motionIndex, const glm::mat4 &parent) {
//  if (children.size() <= 0) {
//    return {};
//  }

//  BoneList ret;
//  const auto transform =
//      CalculateTransform(motions[motionIndex], parent, offset);
//  for (const auto &child : children) {
//    glm::vec4 pos{1.0f, 1.0f, 1.0f, 1.0f};
//    ret.emplace_back(transform * pos, transform * pos);
//    auto childBones = child->ApplyMatriciesRecursive(motionIndex, transform);
//    for (const auto &childBone : childBones) {
//      ret.push_back(childBone);
//    }
//  }
//  return ret;
//}

// std::vector<BoneList> SkeletonNode::ApplyMatricies() {
//  std::vector<BoneList> ret;
//  for (size i = 0; i < motions.size(); i++) {
//    ret.push_back(ApplyMatriciesRecursive(i));
//  }
//  return ret;
//}
