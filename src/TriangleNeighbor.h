//
// Created by Jimmy on 12/14/2019.
//

#pragma once

#include "common.h"

#include <array>
#include <glm/vec3.hpp>
#include <unordered_set>
#include <vector>

struct Edge {
  u32 start;
  u32 end;

  bool operator==(const Edge &o) const { return start == o.start && end == o.end; }
  Edge() = default;
};

struct Triangle {
  std::array<Edge, 3> m_edges;
  std::unordered_set<Triangle *> m_neighbors;
  Triangle() = default;
};

std::vector<Triangle *> ConstructEdgeList(
    const std::vector<f32> &verts, const std::vector<u32> &connections);
