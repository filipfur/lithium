#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace lithium
{
    class SimpleCamera
    {
    public:
        SimpleCamera(const glm::mat4& projection, const glm::vec3& up) : _projection{projection}, _up{up}
        {

        }

        SimpleCamera(const glm::mat4& projection) : SimpleCamera{projection, glm::vec3{0.0f, 1.0f, 0.0f}}
        {

        }

        void setPosition(const glm::vec3 position)
        {
            _position = position;
            _viewChanged = true;
            update();
        }

        void setTarget(const glm::vec3 target)
        {
            _target = target;
            _viewChanged = true;
            update();
        }

        glm::vec3 position() const
        {
            return _position;
        }

        glm::vec3 target() const
        {
            return _target;
        }

        glm::mat4 projection() const
        {
            return _projection;
        }

        glm::mat4 view() const
        {
            return _view;
        }

        void setUp(const glm::vec3& up)
        {
            _up = up;
            _viewChanged = true;
            update();
        }

    private:
        void update()
        {
            if(_viewChanged)
            {
                _view = glm::lookAt(_position, _target, _up);
                _viewChanged = false;
            }
        }

        glm::mat4 _projection;
        glm::mat4 _view;
        glm::vec3 _position{0.0f};
        glm::vec3 _target{0.0f};
        glm::vec3 _up{0.0f, 1.0f, 0.0f};
        //glm::vec3 _direction;
        bool _viewChanged{true};
    };
}
