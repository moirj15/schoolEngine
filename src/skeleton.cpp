#define GLM_ENABLE_EXPERIMENTAL
#include "skeleton.h"

#include "VertexBuffer.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

std::vector<SkeletonNode *> SkeletonNode::ToList() {
  if (children.empty()) {
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

static glm::mat4 CalculateRotations(const NodeMotion &motion) {

  auto rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(motion.rotations.x), {1.0f, 0.0f, 0.0f});
  auto rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(motion.rotations.y), {0.0f, 1.0f, 0.0f});
  auto rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(motion.rotations.z), {0.0f, 0.0f, 1.0f});
  return rotateZ * rotateX * rotateY;
}

void SkeletonNode::Transformations(Size frameCount, const std::vector<glm::mat4> &parent) {
  auto list = ToList();
  for (auto &node : list) {
    for (Size i = 0; i < frameCount; i++) {
      if (!node->parent) {
        glm::mat4 translate = glm::translate(glm::mat4{1.0f}, node->motions[i].translations);
        auto rotation = CalculateRotations(node->motions[i]);
        auto transform = glm::translate(translate, node->offset); // *rotation;
        node->transforms.emplace_back(translate * rotation);
      } else if (node->children.empty()) {
        node->transforms.emplace_back(
            node->parent->transforms[i] * glm::translate(glm::mat4(1.0f), node->offset));
      } else {

        auto parentTransform = node->parent->transforms[i];
        auto rotation = CalculateRotations(node->motions[i]);
        node->transforms.emplace_back(
            parentTransform * glm::translate(glm::mat4(1.0f), node->offset) * rotation);
      }
    }
  }
}

BoneList SkeletonNode::Bones(const glm::vec3 parent) {
  if (children.empty()) {
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

VertexArray *Skeleton::NextTransformedBones(f32 t) {
  static Size transformIndex = 0;
  if (m_skeletonTree->transforms.empty()) {
    m_skeletonTree->Transformations(m_skeletonTree->frames);
  }
  size_t index = ceil(m_skeletonTree->transforms.size() * (t / MaxFrameTime()));
  BoneList transformedBones;
  auto nodes = m_skeletonTree->ToList();
  for (const auto &node : nodes) {
    for (const auto &child : node->children) {
      // transformedBones.emplace_back(
      //    node->transforms[transformIndex] * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
      //    child->transforms[transformIndex] * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
      transformedBones.emplace_back(node->transforms[index % m_skeletonTree->transforms.size()]
                                        * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
          child->transforms[index % m_skeletonTree->transforms.size()]
              * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
    }
  }
  transformIndex = (transformIndex + 1) % m_skeletonTree->frames;
  return BonesToVAO(transformedBones);
}

VertexArray *Skeleton::BonesToVAO(const BoneList &bones) {
  std::vector<f32> points;
  std::vector<u32> indexes;
  for (Size i = 0; i < bones.size(); i++) {
    indexes.push_back(i * 2);
    indexes.push_back((i * 2) + 1);
    for (Size v = 0; v < 3; v++) {
      points.push_back(bones[i].first[v]);
    }
    for (Size v = 0; v < 3; v++) {
      points.push_back(bones[i].second[v]);
    }
  }
  auto *ret = new VertexArray;
  ret->AddIndexBuffer(new IndexBuffer(indexes.data(), indexes.size()));
  ret->AddVertexBuffer(
      new VertexBuffer(points.data(), points.size(), {{"points", 3, 0, 0, GL_FLOAT}}));
  return ret;
}
