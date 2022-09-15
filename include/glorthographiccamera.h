#pragma once

#include "glcamera.h"

namespace lithium
{
    class OrthographicCamera : public lithium::Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
        : lithium::Camera{glm::ortho(left, right, bottom, top, nearPlane, farPlane), glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{0.0, 0.0, 1.0}}
        {
            
        }
        virtual ~OrthographicCamera() noexcept;

        virtual void update(float dt) override;

    private:

        

    };
}