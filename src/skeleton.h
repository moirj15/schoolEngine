#ifndef SKELETON_H
#define SKELETON_H

#include "common.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

struct SkeletonNode {
  glm::vec3 offset;
};

class Skeleton {
  std::vector<std::unique_ptr<Skeleton>> m_children;

public:
  Skeleton();
};

#endif // SKELETON_H
