#include "aabb.h"

#include <algorithm>

bool AABB::IntersectRay(const Ray &ray) {
  f32 txMin = (m_bounds[ray.sign[0]].x - ray.origin.x) * ray.inverseDir.x;
  f32 txMax = (m_bounds[1 - ray.sign[0]].x - ray.origin.x) * ray.inverseDir.x;
  f32 tyMin = (m_bounds[ray.sign[1]].y - ray.origin.y) * ray.inverseDir.y;
  f32 tyMax = (m_bounds[1 - ray.sign[1]].y - ray.origin.y) * ray.inverseDir.y;
  if ((txMin > tyMax) || (tyMin > txMax)) {
    return false;
  }
  if (tyMin > txMin) {
    txMin = tyMin;
  }
  if (tyMax < txMax) {
    txMax = tyMax;
  }
  f32 tzMin = (m_bounds[ray.sign[2]].z - ray.origin.z) * ray.inverseDir.z;
  f32 tzMax = (m_bounds[1 - ray.sign[2]].z - ray.origin.z) * ray.inverseDir.z;
  if ((txMin > tzMax) || (tzMin > txMax)) {
    return false;
  }
  if (tzMin > txMin) {
    txMin = tzMin;
  }
  if (tzMax < txMax) {
    txMax = tzMax;
  }

  return true;
}

bool AABB::IntersectAABB(const AABB &aabb) {
  return false;
}

// struct Triangle {
//  std::array<glm::vec3, 3> m_points;
//  std::array<u32, 3> m_indexes;
//};

// static AABBTree *RecursiveConstructAABBTree(const std::vector<Triangle> &triangles) {
//  auto *tree = new AABBTree();
//  if (triangles.size() == 1) {
//  }
//  return tree;
//}

AABBTree *ConstructAABBTree(const std::vector<f32> &verts, const std::vector<u32> &connections) {
  // std::vector<Triangle> triangles;
  // for (size_t i = 0; i < connections.size(); i += 3) {
  //  Triangle triangle;
  //  u32 c0 = connections[i];
  //  u32 c1 = connections[i + 1];
  //  u32 c2 = connections[i + 2];
  //  triangle.m_indexes[0] = c0;
  //  triangle.m_indexes[1] = c1;
  //  triangle.m_indexes[2] = c2;

  //  triangle.m_points[0] = {verts[c0], verts[c0 + 1], verts[c0 + 2]};
  //  triangle.m_points[1] = {verts[c1], verts[c1 + 1], verts[c1 + 2]};
  //  triangle.m_points[2] = {verts[c2], verts[c2 + 1], verts[c2 + 2]};
  //  triangles.push_back(triangle);
  //}

  // auto *tree = RecursiveConstructAABBTree(triangles);
  // return tree;
  return nullptr;
}
void DeleteAABBTree(AABBTree *tree) {
}
