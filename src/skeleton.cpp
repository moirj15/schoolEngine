#include "skeleton.h"

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
