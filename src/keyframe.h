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
    size_t m_startFrame = 0;
    size_t m_middleFrame = 1;
    size_t m_endFrame = 2;
public:
    KeyFrameGroup() = default;
    void LoadFromFile(const std::string &filename);

    glm::mat4 GenerateTranslationMat(const f32 time);
private:
    void UpdateFramePoints(const f32 t);
};

#endif // KEYFRAME_H
