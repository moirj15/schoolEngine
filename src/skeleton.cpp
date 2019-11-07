#define GLM_ENABLE_EXPERIMENTAL
#include "skeleton.h"

#include "VertexBuffer.h"

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

  //  glm::mat4 motionTransform =
  //      glm::translate(glm::mat4(1.0f), motion.translations);
  glm::mat4 offsetTransform =
      glm::translate(glm::mat4(1.0f), offset + motion.translations);

  auto rotateX = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.x),
      glm::normalize(glm::vec3{1.0f, 0.0f, 0.0f})));

  auto rotateY = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.y),
      glm::normalize(glm::vec3{0.0f, 1.0f, 0.0f})));

  auto rotateZ = glm::toMat4(glm::angleAxis(glm::radians(motion.rotations.z),
      glm::normalize(glm::vec3{0.0f, 0.0f, 1.0f})));

  //  auto transform = rotateZ * rotateY * rotateX * offsetTransform;
  //  auto transform = rotateX * parent;
  //  transform = rotateY * transform;
  //  transform = rotateZ * transform;
  //  auto transform = parent * offsetTransform;
  auto transform = parent * offsetTransform * rotateZ * rotateY * rotateX;
  return transform;
}

// std::vector<glm::mat4> SkeletonNode::Transformations(
//    size i, const glm::mat4 &parent) {
//  if (children.size() <= 0) {
//    return {glm::translate(glm::mat4{1.0f}, offset)};
//  }
//  auto transform = CalculateTransform(motions[i], parent, offset);
//  std::vector<glm::mat4> ret;
//  ret.emplace_back(transform);
//  for (auto &child : children) {
//    auto childTransforms = child->Transformations(i, transform);
//    for (auto &childTransform : childTransforms) {
//      ret.emplace_back(childTransform);
//    }
//  }
//  return ret;
//}
void SkeletonNode::Transformations(
    size frameCount, const std::vector<glm::mat4> &parent) {
  //  if (children.size() <= 0) {
  //    return;
  //  }
  for (size i = 0; i < motions.size(); i++) {
    glm::mat4 parentTransform{1.0f};
    if (!parent.empty()) {
      parentTransform = parent[i];
    }
    auto transform = CalculateTransform(motions[i], parentTransform, offset);
    transforms.emplace_back(transform);
  }
  if (motions.empty()) {
    for (size i = 0; i < frameCount; i++) {
      glm::mat4 parentTransform{1.0f};
      if (!parent.empty()) {
        parentTransform = parent[i];
      }
      auto transform = CalculateTransform(
          {glm::vec3{1.0f}, glm::vec3{1.0f}}, parentTransform, offset);
      transforms.emplace_back(transform);
    }
  }
  for (auto &child : children) {
    child->Transformations(frameCount, transforms);
  }
}

BoneList SkeletonNode::Bones(const glm::vec3 parent) {
  if (children.size() <= 0) {
    return {};
  }
  BoneList ret;
  for (auto &child : children) {
    auto newOffset = offset + parent;
    ret.emplace_back(newOffset, newOffset + child->offset);
    auto childBones = child->Bones(newOffset);
    for (auto &childBone : childBones) {
      ret.emplace_back(childBone);
    }
  }
  return ret;
}

VertexArray *Skeleton::DrawableBones() {
  if (m_bones.empty()) {
    m_bones = m_skeletonTree->Bones();
  }
  return BonesToVAO(m_bones);
}

VertexArray *Skeleton::NextTransformedBones() {
  static size transformIndex = 0;
  if (m_skeletonTree->transforms.empty()) {
    m_skeletonTree->Transformations(m_skeletonTree->frames);
  }
  BoneList transformedBones;
  auto nodeList = m_skeletonTree->ToList();
  std::vector<std::pair<glm::mat4, glm::mat4>> transforms;
  for (const auto *node : nodeList) {
    for (const auto &child : node->children) {
      //      if (!child->.empty()) {
      transforms.emplace_back(
          node->transforms[transformIndex], child->transforms[transformIndex]);
      //      }
    }
  }

  for (const auto tPair : transforms) {
    transformedBones.emplace_back(
        tPair.first * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
        tPair.first * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
  }
  //  for (size i = 0; i < m_bones.size(); i++) {
  //    //    const auto &firstTransform = m_transforms[transformIndex][i];
  //    //    const auto &secondTransform = m_transforms[transformIndex][i + 1];
  //    //    const auto &bone = m_bones[i];
  //    transformedBones.emplace_back(
  //        firstTransform * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
  //        secondTransform * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
  //  }
  transformIndex = (transformIndex + 1) % m_skeletonTree->transforms.size();

  auto b = m_skeletonTree->Bones();
  return BonesToVAO(transformedBones);
}

VertexArray *Skeleton::BonesToVAO(const BoneList &bones) {
  std::vector<f32> points;
  std::vector<u32> indexes;
  for (size i = 0; i < bones.size(); i++) {
    indexes.push_back(i * 2);
    indexes.push_back((i * 2) + 1);
    for (size v = 0; v < 3; v++) {
      points.push_back(bones[i].first[v]);
    }
    for (size v = 0; v < 3; v++) {
      points.push_back(bones[i].second[v]);
    }
  }
  auto *ret = new VertexArray;
  ret->AddIndexBuffer(new IndexBuffer(indexes.data(), indexes.size()));
  ret->AddVertexBuffer(new VertexBuffer(
      points.data(), points.size(), {{"points", 3, 0, 0, GL_FLOAT}}));
  return ret;
}
