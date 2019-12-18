#ifndef SKELETON_H
#define SKELETON_H

#include "common.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class VertexArray;

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
  std::vector<glm::mat4> transforms;
  std::vector<std::unique_ptr<SkeletonNode>> children = {};
  SkeletonNode *parent = nullptr;
  Size frames = 0;
  f32 frameTime = 0.0f;

  /**
   * Consturctor.
   * @param n: The name of the node
   */
  SkeletonNode(const std::string &n) : name{n} {}

  std::vector<SkeletonNode *> ToList();

  /**
   * Applies the given transformations.
   */
  void Transformations(Size frameCount, const std::vector<glm::mat4> &parent = {});

  /**
   * Recursively builds a boneList.
   * @param parent: The parent offest.
   * @return: The boneList.
   */
  BoneList Bones(const glm::vec3 parent = glm::vec3(0.0f));

private:
  /**
   * Recursively applies the matricies.
   * @param motionIndex: THe current motion index.
   * @param parent: the current parent transform.
   * @return : the bone list.
   */
  BoneList ApplyMatriciesRecursive(Size motionIndex, const glm::mat4 &parent = glm::mat4(1.0f));
};

class Skeleton {
  std::unique_ptr<SkeletonNode> m_skeletonTree;
  BoneList m_bones;
  std::vector<std::vector<glm::mat4>> m_transforms;
  Size m_motionCount;

public:
  /**
   * Constructor.
   * @param nodes: The Skeleton tree made up of skeleton nodes.
   */
  Skeleton(SkeletonNode *nodes) :
      m_skeletonTree{nodes}, m_bones{}, m_transforms{}, m_motionCount{nodes->motions.size()} {}

  /// Creates a VertexArray containing the bones.
  VertexArray *DrawableBones();

  /// Creates a VertexArray using the next bones transforms.
  VertexArray *NextTransformedBones();

  BoneList Bones() const { return m_bones; }
  f32 FrameTime() const { return m_skeletonTree->frameTime; }

private:
  void PopulateBones();
  VertexArray *BonesToVAO(const BoneList &bones);
};

#endif // SKELETON_H
