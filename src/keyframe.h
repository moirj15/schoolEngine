#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "common.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
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
  /// Constructor
  KeyFrameGroup() = default;

  /**
   * Loads keyframes from the given file.
   * @param filename: The name of the file containing the keyframe information.
   */
  void LoadFromFile(const std::string &filename);

  /**
   * Generates the transformation matrix that belongs to the given time.
   * @param time: The time.
   * @return the transformation matrix.
   */
  glm::mat4 GenerateTranslationMat(const f32 time);

private:
  /// Updates the frame points with the givne time.
  void UpdateFramePoints(const f32 t);
};

#endif // KEYFRAME_H
