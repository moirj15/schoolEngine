#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
struct KeyFrame;

glm::vec3 DCCSpline(const f32 u, const glm::vec3 &start, const glm::vec3 &p, const glm::vec3 &end);

glm::quat DCCSpline(const f32 u, const KeyFrame &start, const KeyFrame &p, const KeyFrame &end);

#endif