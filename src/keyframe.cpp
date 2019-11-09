#include "keyframe.h"

#include "interpolation.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

void KeyFrameGroup::LoadFromFile(const std::string &filename) {
  FILE *file = OpenFile(filename.c_str(), "r");
  char buffer[256] = {};
  while (fgets(buffer, sizeof(buffer), file) != nullptr) {
    KeyFrame keyFrame{};
    sscanf(buffer, "%f %f %f %f %f %f %f %f\n", &keyFrame.time, &keyFrame.pos.x, &keyFrame.pos.y,
        &keyFrame.pos.z, &keyFrame.axis.x, &keyFrame.axis.y, &keyFrame.axis.z, &keyFrame.angle);
    m_keyFrames.push_back(keyFrame);
    memset(buffer, 0, sizeof(buffer));
  }
  fclose(file);
}

glm::mat4 KeyFrameGroup::GenerateTranslationMat(const f32 time) {
  UpdateFramePoints(time);
  const f32 t = time - m_timeFrameStart;
  glm::vec3 newPos;
  glm::quat newRot;
  if (m_endFrame >= m_keyFrames.size()) {
    f32 u = (t - m_keyFrames[m_startFrame].time)
            / (m_keyFrames[m_middleFrame].time - m_keyFrames[m_startFrame].time);

    newPos = DCCSpline(u, m_keyFrames[m_startFrame].pos, m_keyFrames[m_middleFrame].pos,
        m_keyFrames[m_middleFrame].pos);

    newRot = DCCSpline(
        u, m_keyFrames[m_startFrame], m_keyFrames[m_middleFrame], m_keyFrames[m_middleFrame]);
  } else {
    f32 u = (t - m_keyFrames[m_startFrame].time)
            / (m_keyFrames[m_endFrame].time - m_keyFrames[m_startFrame].time);

    newPos = DCCSpline(u, m_keyFrames[m_startFrame].pos, m_keyFrames[m_middleFrame].pos,
        m_keyFrames[m_endFrame].pos);

    newRot = DCCSpline(
        u, m_keyFrames[m_startFrame], m_keyFrames[m_middleFrame], m_keyFrames[m_endFrame]);
  }
  return glm::translate(glm::mat4(1.0f), newPos) * glm::toMat4(newRot);
}

// Private
void KeyFrameGroup::UpdateFramePoints(const f32 t) {
  Size endFrame = m_endFrame;
  if (m_endFrame >= m_keyFrames.size()) {
    endFrame = m_middleFrame;
  }
  if (t <= m_keyFrames[endFrame].time + m_timeFrameStart) {
    return;
  }
  if (t >= m_keyFrames.back().time + m_timeFrameStart) {
    m_timeFrameStart += m_keyFrames.back().time;
    m_startFrame = 0;
    m_middleFrame = 1;
    m_endFrame = 2;
    return;
  }
  m_startFrame += 2;
  m_middleFrame += 2;
  m_endFrame += 2;
}
