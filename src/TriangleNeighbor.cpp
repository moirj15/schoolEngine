//
// Created by Jimmy on 12/14/2019.
//

#include "TriangleNeighbor.h"

#include <unordered_map>
size_t edgeHash(const Edge &edge) {
  return std::hash<u32>()(edge.start) + std::hash<u32>()(edge.end);
}

// bool triangleEq(const Triangle &a, const Triangle &b) {
//  return (a.m_edges[0] == b.m_edges[0]) && (a.m_edges[1] == b.m_edges[1])
//         && (a.m_edges[2] == b.m_edges[2]);
//}

std::vector<Triangle *> ConstructEdgeList(
    const std::vector<f32> &verts, const std::vector<u32> &connections) {
  auto edgeHash = [](const Edge &e) { return e.start + e.end; };
  auto edgeEq = [](const Edge &a, const Edge &b) { return a == b; };
  std::unordered_map<Edge, std::vector<Triangle *>, decltype(edgeHash), decltype(edgeEq)>
      triangleMap(connections.size(), edgeHash, edgeEq);
  std::vector<Triangle *> triangles;
  for (size_t i = 0; i < connections.size(); i += 3) {
    Triangle *triangle = new Triangle();
    u32 c0 = connections[i];
    u32 c1 = connections[i + 1];
    u32 c2 = connections[i + 2];
    triangle->m_edges[0] = {c0, c1};
    triangle->m_edges[1] = {c1, c2};
    triangle->m_edges[2] = {c2, c0};
    triangles.push_back(triangle);
    triangleMap[triangle->m_edges[0]].push_back(triangle);
    triangleMap[triangle->m_edges[1]].push_back(triangle);
    triangleMap[triangle->m_edges[2]].push_back(triangle);
  }
  for (auto &pair : triangleMap) {
    auto &neighbors = pair.second;
    if (neighbors.size() > 1) {
      neighbors[0]->m_neighbors.emplace(neighbors[1]);
      neighbors[1]->m_neighbors.emplace(neighbors[0]);
    }
  }

  return triangles;
}
