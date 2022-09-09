#pragma once

#include "glcamera.h"

namespace mygl
{
    class PerspectiveCamera : public mygl::Camera
    {
    public:
        PerspectiveCamera(float width, float height, const glm::vec3& position, float fovDegrees, float nearPlane, float farPlane)
            : mygl::Camera{glm::perspective(glm::radians(fovDegrees), width / height, nearPlane, farPlane),
            glm::vec3{0.0f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f}}
        {
            
        }

        virtual ~PerspectiveCamera() noexcept;

        virtual void update(float dt) override;

    private:
    };
}