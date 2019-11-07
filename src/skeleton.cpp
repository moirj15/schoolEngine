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

static glm::mat4 CalculateTransform(const NodeMotion &motion,
    const glm::mat4 &parent, const glm::vec3 &offset) {

  glm::mat4 motionTransform =
      glm::translate(glm::mat4(1.0f), motion.translations);
  glm::mat4 offsetTransform = glm::translate(motionTransform, offset);

  auto rotateX = glm::rotate(
      glm::mat4(1.0f), glm::radians(motion.rotations.x), {1.0f, 0.0f, 0.0f});
  auto rotateY = glm::rotate(
      glm::mat4(1.0f), glm::radians(motion.rotations.y), {0.0f, 1.0f, 0.0f});
  auto rotateZ = glm::rotate(
      glm::mat4(1.0f), glm::radians(motion.rotations.z), {0.0f, 0.0f, 1.0f});

  auto transform = parent * offsetTransform;
  // auto transform = parent * offsetTransform * rotateZ * rotateY * rotateX;
  return transform;
}

// void SkeletonNode::Transformations(
//    size frameCount, const std::vector<glm::mat4> &parent) {
//  for (size i = 0; i < motions.size(); i++) {
//    glm::mat4 parentTransform{1.0f};
//    if (!parent.empty()) {
//      parentTransform = parent[i];
//    }
//    auto transform = CalculateTransform(motions[i], parentTransform, offset);
//    transforms.emplace_back(transform);
//  }
//  if (motions.empty()) {
//    for (size i = 0; i < frameCount; i++) {
//      glm::mat4 parentTransform{1.0f};
//      if (!parent.empty()) {
//        parentTransform = parent[i];
//      }
//      auto transform = CalculateTransform(
//          {glm::vec3{1.0f}, glm::vec3{1.0f}}, parentTransform, offset);
//      transforms.emplace_back(transform);
//    }
//  }
//  for (auto &child : children) {
//    child->Transformations(frameCount, transforms);
//  }
//}

static glm::mat4 CalculateRotations(const NodeMotion &motion) {

  auto rotateX = glm::rotate(
      glm::mat4(1.0f), glm::radians(motion.rotations.x), {1.0f, 0.0f, 0.0f});
  auto rotateY = glm::rotate(
      glm::mat4(1.0f), glm::radians(motion.rotations.y), {0.0f, 1.0f, 0.0f});
  auto rotateZ = glm::rotate(
      glm::mat4(1.0f), glm::radians(motion.rotations.z), {0.0f, 0.0f, 1.0f});
  return rotateZ * rotateY * rotateX;
}

void SkeletonNode::Transformations(
    size frameCount, const std::vector<glm::mat4> &parent) {
  auto list = ToList();
  for (auto &node : list) {
    //if (node->parent) {
    //  node->offset += node->parent->offset;
    //}
    for (size i = 0; i < frameCount; i++) {
      if (!node->parent) {
        glm::mat4 translate =
            glm::translate(glm::mat4{1.0f}, node->motions[i].translations);
        auto rotation = CalculateRotations(node->motions[i]);
        auto transform = glm::translate(translate, node->offset);// *rotation;
        node->transforms.emplace_back(translate * rotation);
      } else if (node->children.empty()) {
        //node->transforms.emplace_back(node->parent->transforms[i] * glm::translate(glm::mat4(1.0f), node->offset) * CalculateRotations(node->parent->motions[i]));
        node->transforms.emplace_back(node->parent->transforms[i] * glm::translate(glm::mat4(1.0f), node->offset));
      } else {
        
        auto parentTransform = node->parent->transforms[i];
        auto rotation = CalculateRotations(node->motions[i]);
        node->transforms.emplace_back(parentTransform * glm::translate(glm::mat4(1.0f), node->offset) * rotation);
        //node->transforms.emplace_back(parentTransform * glm::translate(glm::mat4(1.0f), node->offset));
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
// VertexArray *Skeleton::NextTransformedBones() {
//  static size transformIndex = 0;
//  if (m_skeletonTree->transforms.empty()) {
//    m_skeletonTree->Transformations(m_skeletonTree->frames);
//  }
//  BoneList transformedBones;
//  auto nodeList = m_skeletonTree->ToList();
//  std::vector<std::pair<glm::mat4, glm::mat4>> transforms;
//  for (const auto *node : nodeList) {
//    for (const auto &child : node->children) {
//      transforms.emplace_back(
//          node->transforms[transformIndex],
//          child->transforms[transformIndex]);
//    }
//  }
//
//  for (const auto tPair : transforms) {
//    transformedBones.emplace_back(
//        tPair.first * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f},
//        tPair.first * glm::vec4{1.0f, 1.0f, 1.0f, 1.0f});
//  }
//  transformIndex = (transformIndex + 1) % m_skeletonTree->transforms.size();
//
//  auto b = m_skeletonTree->Bones();
//  return BonesToVAO(transformedBones);
//}

VertexArray *Skeleton::NextTransformedBones() {
  static size transformIndex = 0;
  if (m_skeletonTree->transforms.empty()) {
    m_skeletonTree->Transformations(m_skeletonTree->frames);
  }
  BoneList transformedBones;
  auto nodes = m_skeletonTree->ToList();
  for (const auto &node : nodes) {
    for (const auto &child : node->children) {
      //transformedBones.emplace_back(
      //    node->transforms[transformIndex] * glm::vec4{node->offset, 1.0f},
      //    child->transforms[transformIndex] * glm::vec4{child->offset, 1.0f});
      transformedBones.emplace_back(
          node->transforms[transformIndex] * glm::vec4{1.0f,1.0f,1.0f, 1.0f},
          child->transforms[transformIndex] * glm::vec4{1.0f,1.0f,1.0f, 1.0f});
    }
  }
  transformIndex = (transformIndex + 1) % m_skeletonTree->frames;
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
