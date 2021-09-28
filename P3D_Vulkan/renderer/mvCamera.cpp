#include "mvCamera.h"
#include <algorithm>

namespace DearPy3D {

    template<typename T>
    static T wrap_angle(T theta) noexcept
    {
        constexpr T twoPi = (T)2 * (T)PI_D;
        const T mod = (T)fmod(theta, twoPi);
        if (mod > (T)PI_D)
            return mod - twoPi;
        else if (mod < -(T)PI_D)
            return mod + twoPi;
        return mod;
    }

    glm::mat4 mvBuildCameraMatrix(const mvCamera& camera)
    {
        glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(camera.yaw, camera.pitch, 0.0f);

        glm::vec4 forward_base_vector = { 0.0f, 0.0f, 1.0f, 0.0f };

        glm::vec4 look_vector = roll_pitch_yaw * forward_base_vector;

        glm::vec3 lpos = { look_vector.x, look_vector.y, look_vector.z };

        glm::vec3 look_target = camera.pos + lpos;

        glm::mat4 camera_matrix = glm::lookAt(camera.pos, look_target, glm::vec3{ 0.0f, -1.0f, 0.0f });

        return camera_matrix;
    }

    glm::mat4 mvBuildProjectionMatrix(const mvCamera& camera)
    {
        return glm::perspective(glm::radians(45.0f), camera.aspect, 0.1f, 400.0f);
    }

    void mvRotateCamera(mvCamera& camera, float dx, float dy)
    {
        camera.yaw = wrap_angle(camera.yaw + dx * CameraRotationSpeed);
        camera.pitch = std::clamp(camera.pitch + dy * CameraRotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
    }

    void mvTranslateCamera(mvCamera& camera, float dx, float dy, float dz)
    {
        glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(camera.yaw, camera.pitch, 0.0f);
        glm::mat4 scale = glm::scale(glm::vec3{ CameraTravelSpeed, CameraTravelSpeed, CameraTravelSpeed });
        glm::vec4 translation = glm::vec4{ dx, dy, dz, 0.0f };

        translation = (roll_pitch_yaw * scale) * translation;

        camera.pos = {
            camera.pos.x + translation.x,
            camera.pos.y + translation.y,
            camera.pos.z + translation.z
        };
    }

}
