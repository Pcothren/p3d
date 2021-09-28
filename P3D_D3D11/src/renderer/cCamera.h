#pragma once

#include "cMath.h"

namespace p3d {

    static constexpr float CameraTravelSpeed = 12.0f;
    static constexpr float CameraRotationSpeed = 0.004f;

    struct cCamera
    {
        glm::vec3 pos;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float aspect = 1.0f;
    };

    glm::mat4 cBuildCameraMatrix    (const cCamera& camera);
    glm::mat4 cBuildProjectionMatrix(const cCamera& camera);
    void      cRotateCamera         (cCamera& camera, float dx, float dy);
    void      cTranslateCamera      (cCamera& camera, float dx, float dy, float dz);

}
