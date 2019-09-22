#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include <glm/glm.hpp>
#include "common.h"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 inverseDir;
    s32 sign[3];

    Ray(const glm::vec3 &orig, const glm::vec3 &dir) :
            origin(orig), direction(dir) {
        inverseDir = 1.0f / dir;
        sign[0] = inverseDir.x < 0;
        sign[1] = inverseDir.y < 0;
        sign[2] = inverseDir.z < 0;
    }
};

struct BoundingBox {
    glm::vec3 m_bounds[2];
public:
    BoundingBox() = default;
    BoundingBox(const glm::vec3 &a, const glm::vec3  &b) :
        m_bounds{a, b} {}

    bool IntersectRay(const Ray &ray);
};

#endif // BOUNDINGBOX_H
