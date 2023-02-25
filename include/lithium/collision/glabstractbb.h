#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace lithium
{
    class AbstractBB
    {
    public:
        AbstractBB(const glm::vec3 position) : _position{position}
        {

        }

        AbstractBB(const AbstractBB& other) : _position{other._position}
        {

        }

        virtual ~AbstractBB() noexcept
        {

        }

        glm::vec3 position() const
        {
            return _position;
        }

        void setPosition(const glm::vec3& position)
        {
            _position = position;
        }

    protected:
        glm::vec3 _position;
    };
}