#pragma once

#include "glcamera.h"

namespace lithium
{
    class OrthographicCamera : public lithium::Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        virtual ~OrthographicCamera() noexcept;

    };
}