#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <string>
#include <vector>
#include <array>

#include <glm/glm.hpp>
#include "common.h"


struct KeyFrame {
    f32 time;
    glm::vec3 pos;
    glm::vec3 axis;
    f32 angle;
};

class KeyFrameGroup {
    std::vector<KeyFrame> m_keyFrames = {};
public:
    KeyFrameGroup() = default;
    void LoadFromFile(const std::string &filename);

    glm::mat4 GenerateTranslationMat(const f32 time);
private:
    std::array<KeyFrame, 2> GenerateControlPoints(const glm::vec3 &start, const glm::vec3 &end);
};

#endif // KEYFRAME_H
