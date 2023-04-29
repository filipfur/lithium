#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "glcollision.h"

namespace lithium
{
    class Geometry
    {
    public:
        Geometry(const glm::vec3 position) : _position{position}
        {

        }

        Geometry(const Geometry& other) : _position{other._position}
        {

        }

        Geometry& operator=(const Geometry& other)
        {
            _position = other._position;
            return *this;
        }

        Geometry(Geometry&& other) noexcept : _position{std::move(other._position)}
        {

        }

        Geometry& operator=(Geometry&& other) noexcept
        {
            _position = std::move(other._position);
            return *this;
        }

        virtual ~Geometry() noexcept
        {
            for(Geometry* child : _children)
            {
                delete child;
            }
            _children.clear();
        }

        glm::vec3 position() const
        {
            return _position;
        }

        void setPosition(const glm::vec3& position)
        {
            _position = position;
        }

        void setActive(bool active)
        {
            _active = active;
        }

        bool active() const
        {
            return _active;
        }

        void addChild(Geometry* child)
        {
            _children.push_back(child);
        }

        void forEachChild(const std::function<void(Geometry*)>& func)
        {
            for(Geometry* child : _children)
            {
                func(child);
            }
        }

        bool recursiveTest(const Geometry& other, Collision& collision) const
        {
            if(other.active() && this->test(other, collision))
            {
                if(_children.size() == 0)
                {
                    return true;
                }
                else
                {
                    for(Geometry* child : _children)
                    {
                        if(child->recursiveTest(other, collision))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        //virtual bool intersect(const Geometry& other) const = 0;
        virtual bool intersect(const glm::vec3& point) const = 0;
        virtual bool intersect(const class AABB& aabb, Collision& collision) const = 0;
        virtual bool intersect(const class Sphere& sphere, Collision& collision) const = 0;
        virtual bool test(const Geometry& other, Collision& collision) const = 0;

    protected:
        bool _active{true};
        glm::vec3 _position;
        std::vector<Geometry*> _children;
    };
}