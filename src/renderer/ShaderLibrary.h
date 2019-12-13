//
// Created by Jimmy on 12/13/2019.
//

#pragma once

#include "../common.h"

#include <unordered_map>

namespace renderer {

class Shader;

class ShaderLibrary {
  std::unordered_map<std::string, Shader *> m_shaders;

public:
  ShaderLibrary();
  ~ShaderLibrary();

  void Add(const std::string &name);
  void Remove();
};

} // namespace renderer
