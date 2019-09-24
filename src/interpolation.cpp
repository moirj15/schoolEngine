#include "interpolation.h"
#include <array>

    //glm::mat4 basis{-1.0f,  3.0f, -3.0f, 1.0f,
    //                 3.0f, -6.0f, -4.0f, 0.0f,
    //                -3.0f,  3.0f,  0.0f, 0.0f,
    //                 1.0f,  0.0f,  0.0f, 0.0f};
static std::array<glm::vec3, 4> CreateControlPoints(const glm::vec3 &start, const glm::vec3 &p, 
        const glm::vec3 &end) {
    std::array<glm::vec3, 4> controlPoints;
    controlPoints[0] = (p - end) + p;
    // the control point after p has to be calculated before the control point before p
    controlPoints[2] = (((p - start) + p) + end) / 2.0f;
    controlPoints[1] = (p - controlPoints[2]) + p;
    controlPoints[3] = (end - p) + end;
    return controlPoints;
}

/**
 * Implementation of linear interpolation, because glm only has lerp implemented for quaternions.
 * 
 */
static inline glm::vec3 lerp(const f32 u, const glm::vec3 &p0, const glm::vec3 &p1) {
    return (p1 - p0) * u + p0;
}


glm::vec3 DCCSpline(const f32 u, const glm::vec3 &start, const glm::vec3 &p, 
        const glm::vec3 &end) {
    auto controlPoints = CreateControlPoints(start, p, end);
    glm::vec3 q0 = lerp(u, controlPoints[0], controlPoints[1]);
    glm::vec3 q1 = lerp(u, controlPoints[1], controlPoints[2]);
    glm::vec3 q2 = lerp(u, controlPoints[2], controlPoints[3]);
    glm::vec3 r0 = lerp(u, q0, q1);
    glm::vec3 r1 = lerp(u, q1, q2);
    return lerp(u, r0, r1); 
}

glm::quat DCCSpline(const f32 u, const glm::quat &start, const glm::quat &p, const glm::quat &end) {
    return {};
}
