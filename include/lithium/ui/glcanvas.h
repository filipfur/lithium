#pragma once

#include "glframe.h"

namespace lithium
{
    class Canvas : public Frame
    {
    public:
        Canvas(const glm::vec2& resolution, const glm::vec2& dimension);

        virtual ~Canvas() noexcept;

        void move(const glm::vec2& delta)
        {
            _lookTarget += glm::vec3{delta, 0.0f};
        }

        virtual void update(float dt)
        {
            lithium::Frame::update(dt);
            if(glm::distance2(_lookPosition, _lookTarget) < 0.0001f)
            {
                _lookPosition = _lookTarget;
            }
            else
            {
                _lookPosition = glm::mix(_lookPosition, _lookTarget, dt * 16.0f);
                //refresh();
            }
            _camera.setPosition(glm::vec3{0.0f, 0.0f, 1.0f} + _lookPosition);
            _camera.setTarget(_lookPosition);
        }
        
        void renderCanvas();

    private:
        glm::vec2 _resolution;
        SimpleCamera _camera;
        UniformBufferObject _canvasUBO;
        std::shared_ptr<Mesh> _frameMesh;
        glm::vec3 _lookTarget{0.0f};
        glm::vec3 _lookPosition{0.0f};
    };
}