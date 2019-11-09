#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "common.h"

#include <array>
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct KeyFrame {
  f32 time;
  glm::vec3 pos;
  glm::vec3 axis;
  f32 angle;
};

class KeyFrameGroup {
  std::vector<KeyFrame> m_keyFrames = {};
  Size m_startFrame = 0;
  Size m_middleFrame = 1;
  Size m_endFrame = 2;
  f32 m_timeFrameStart = 0.0;

public:
  KeyFrameGroup() = default;
  void LoadFromFile(const std::string &filename);

  glm::mat4 GenerateTranslationMat(const f32 time);

private:
  void UpdateFramePoints(const f32 t);
};

#endif // KEYFRAME_H
