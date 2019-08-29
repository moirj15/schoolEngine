#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "common.h"


class Shader {
    u32 m_programHandle;
    std::vector<std::string> m_filePaths;
    std::unordered_map<std::string, u32> m_uniformCache;
public:
    explicit Shader(const std::vector<std::string> &filePaths);

    inline void Bind() const { glUseProgram(m_programHandle); }
    inline void Unbind() const { glUseProgram(0); }

    inline u32 Handle() const { return m_programHandle; }

    void SetUniform1F(const std::string &name, const f32 value);
    void SetUniform2F(const std::string &name, const glm::vec2 &value);
    void SetUniform3F(const std::string &name, const glm::vec3 &value);
    void SetUniform4F(const std::string &name, const glm::vec4 &value);

    void SetUniformMat4(const std::string &name, const glm::mat4 &value);

private:
    s32 LocationFromCache(const std::string &name);
    void CompileShader();
    std::vector<GLenum> DetermineShaderTypes();
};




#endif
