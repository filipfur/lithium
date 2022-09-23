#pragma once

#include <glm/glm.hpp>

#include "iupdateable.h"

namespace lithium
{
    class SimpleCamera : public IUpdateable
    {
    public:
        SimpleCamera(const glm::mat4& projection) : _projection{projection}
        {

        }

        void setPosition(const glm::vec3 position)
        {
            _position = position;
            _viewChanged = true;
        }

        void setTarget(const glm::vec3 target)
        {
            _target = target;
            _viewChanged = true;
        }

        virtual void update(float dt) override
        {
            if(_viewChanged)
            {
                //_direction = glm::normalize(_position - _target);
                static const glm::vec3 up{0.0f, 1.0f, 0.0f};
                //glm::vec3 cameraRight = glm::normalize(glm::cross(up, _direction));
                //glm::vec3 cameraUp = glm::cross(_direction, cameraRight);
                _view = glm::lookAt(_position, _target, up);

                _viewChanged = false;
            }
        }

        glm::vec3 position() const
        {
            return _position;
        }

        glm::mat4 projection() const
        {
            return _projection;
        }

        glm::mat4 view() const
        {
            return _view;
        }

    private:
        glm::mat4 _projection;
        glm::mat4 _view;
        glm::vec3 _position{0.0};
        glm::vec3 _target{0.0};
        //glm::vec3 _direction;
        bool _viewChanged{true};
    };
}
