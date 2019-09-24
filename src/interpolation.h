#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "common.h"

/**
 * 
 * @param time:
 * @param start:
 * @param p:
 * 
 * @return:
 */
glm::vec3 DCCSpline(const f32 u, const glm::vec3 &start, const glm::vec3 &p, 
    const glm::vec3 &end);

glm::quat DCCSpline(const f32 u, const glm::quat &start, const glm::quat &p, 
    const glm::quat &end);


#endif