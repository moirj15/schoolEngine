#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <glm/glm.hpp>

struct Mesh;

struct Object {
    glm::vec3 m_pos;
    glm::quat m_orientation;
    Mesh *m_mesh;
};

class DynamicObject {
    glm::vec3 m_pos;
    glm::quat m_orientation;
    Mesh *m_mesh;
public:
    virtual ~DynamicObject();

    virtual void Update();
    virtual void CheckCollision();

    const Mesh *ObjectMesh() const { return m_mesh; }
};


#endif
