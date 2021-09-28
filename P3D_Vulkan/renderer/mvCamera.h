#pragma once

#include "mvMath.h"

namespace DearPy3D {

   static constexpr float CameraTravelSpeed = 12.0f;
   static constexpr float CameraRotationSpeed = 0.004f;

    struct mvCamera
    {
        glm::vec3 pos;
        float pitch = 0.0f;
        float yaw = 0.0f;
        float aspect = 1.0f;
    };

    glm::mat4 mvBuildCameraMatrix    (const mvCamera& camera);
    glm::mat4 mvBuildProjectionMatrix(const mvCamera& camera);
    void      mvRotateCamera         (mvCamera& camera, float dx, float dy);
    void      mvTranslateCamera      (mvCamera& camera, float dx, float dy, float dz);

}
