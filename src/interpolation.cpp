#include "interpolation.h"

#include "keyframe.h"

#include <array>
#include <cstdio>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
template<typename T>
static std::array<T, 4> CreateControlPoints(const T &start, const T &p, const T &end) {
  std::array<T, 4> controlPoints{};
  controlPoints[0] = start; //(p - end) + p;
  // the control point after p has to be calculated before the control point
  // before p
  //  controlPoints[2] = ((p + (p - start)) + end) / 2.0f;
  //  controlPoints[1] = (p - controlPoints[2]) + p;
  controlPoints[3] = end; //(end - p) + end;
  return controlPoints;
}

static inline glm::vec3 lerp(const f32 u, const glm::vec3 &p0, const glm::vec3 &p1) {
  return (p1 - p0) * u + p0;
}

glm::vec3 DCCSpline(const f32 u, const glm::vec3 &start, const glm::vec3 &p, const glm::vec3 &end) {
  const auto controlPoints = CreateControlPoints(start, p, end);
  const glm::vec3 q0 = lerp(u, controlPoints[0], controlPoints[1]);
  const glm::vec3 q1 = lerp(u, controlPoints[1], controlPoints[2]);
  const glm::vec3 q2 = lerp(u, controlPoints[2], controlPoints[3]);
  const glm::vec3 r0 = lerp(u, q0, q1);
  const glm::vec3 r1 = lerp(u, q1, q2);
  return lerp(u, r0, r1);
}

glm::quat DCCSpline(const f32 u, const KeyFrame &start, const KeyFrame &p, const KeyFrame &end) {
  glm::quat startQuat{glm::angleAxis((f32)glm::radians(start.angle), glm::normalize(start.axis))};
  glm::quat pQuat{glm::angleAxis((f32)glm::radians(p.angle), glm::normalize(p.axis))};
  glm::quat endQuat{glm::angleAxis((f32)glm::radians(end.angle), glm::normalize(end.axis))};

  const auto controlPoints = CreateControlPoints(startQuat, pQuat, endQuat);
  const auto q0 = glm::normalize(glm::slerp(controlPoints[0], controlPoints[1], u));
  const auto q1 = glm::normalize(glm::slerp(controlPoints[1], controlPoints[2], u));
  const auto q2 = glm::normalize(glm::slerp(controlPoints[2], controlPoints[3], u));
  const auto r0 = glm::normalize(glm::slerp(q0, q1, u));
  const auto r1 = glm::normalize(glm::slerp(q1, q2, u));
  return glm::normalize(glm::slerp(r0, r1, u));
}
