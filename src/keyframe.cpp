#include "keyframe.h"
#include "interpolation.h"


void KeyFrameGroup::LoadFromFile(const std::string &filename) {
    FILE *file = OpenFile(filename.c_str(), "r");
    char buffer[256] = {};
    while (fgets(buffer, sizeof(buffer), file) != nullptr) {
        KeyFrame keyFrame{};
        sscanf(buffer, "%f %f %f %f %f %f %f %f\n", &keyFrame.time, &keyFrame.pos.x,
               &keyFrame.pos.y, &keyFrame.pos.z, &keyFrame.axis.x, &keyFrame.axis.y,
               &keyFrame.axis.z, &keyFrame.angle);
        m_keyFrames.push_back(keyFrame);
        memset(buffer, 0, sizeof(buffer));
    }
    fclose(file);
}

glm::mat4 KeyFrameGroup::GenerateTranslationMat(const f32 time) {
    UpdateFramePoints(time);
    f32 u = (time - m_keyFrames[m_startFrame].time);
    glm::vec3 newPos = DCCSpline(u, m_keyFrames[m_startFrame].pos, m_keyFrames[m_middleFrame].pos,
        m_keyFrames[m_endFrame].pos);
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), newPos);
    return translation;
}

void KeyFrameGroup::UpdateFramePoints(const f32 t) {
    if ((t <= m_keyFrames[m_endFrame].time) || ((m_endFrame + 1) > m_keyFrames.size())) { 
        return; 
    }
    m_startFrame += 2;
    m_middleFrame += 2;
    m_endFrame += 2;
}

// Private
