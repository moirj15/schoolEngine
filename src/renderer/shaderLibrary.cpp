//
// Created by Jimmy on 12/13/2019.
//

#include "shaderLibrary.h"

#include "shader.h"

namespace renderer {
std::string ShaderLibrary::Add(const std::vector<std::string> &filePaths) {
  auto *shader = new Shader(filePaths);
  auto name = filePaths.back().substr(filePaths.back().find_last_of("/"));
  m_shaders.emplace(name, shader);
  return name;
}

void ShaderLibrary::Remove(const std::string &name) {
  m_shaders.erase(name);
}

}

