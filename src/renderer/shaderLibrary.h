//
// Created by Jimmy on 12/13/2019.
//

#pragma once

#include "../common.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace renderer {

class Shader;

// TODO: consider making this a component.
class ShaderLibrary {
  std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;

public:
  std::string Add(const std::vector<std::string> &filePaths);
  void Remove(const std::string &name);

  Shader *GetProgram(const std::string &name) { return m_shaders[name].get(); }
};

} // namespace renderer
