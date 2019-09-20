#include "keyframe.h"


void KeyFrameGroup::LoadFromFile(const std::string &filename) {
    FILE *file = OpenFile(filename.c_str(), "r");
    char buffer[256] = {};
    while (fgets(buffer, sizeof(buffer), file) != nullptr) {
        KeyFrame keyFrame{};
        sscanf(buffer, "%f, %f, %f, %f, %f, %f, %f, %f\n", &keyFrame.time, &keyFrame.pos.x,
               &keyFrame.pos.y, &keyFrame.pos.z, &keyFrame.axis.x, &keyFrame.axis.y,
               &keyFrame.axis.z, &keyFrame.angle);
        m_keyFrames.emplace_back(keyFrame);
        memset(buffer, 0, sizeof(buffer));
    }
    fclose(file);
}

glm::mat4 KeyFrameGroup::GenerateTranslationMat(const f32 time) {

    return {};
}

// Private
std::array<KeyFrame, 2> KeyFrameGroup::GenerateControlPoints(const glm::vec3 &start,
        const glm::vec3 &end) {
    return {};
}
