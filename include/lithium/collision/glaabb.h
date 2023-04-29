#pragma once

#include "glgeometry.h"

namespace lithium
{
    class AABB : public Geometry
    {
    public:
        AABB(const glm::vec3 &position, const glm::vec3 &a, const glm::vec3 &b)
            : Geometry{position}, _a{a}, _b{b},
              _dimensions{abs(b - a)},
              _halfExtents{_dimensions * 0.5f},
              _center{position + a + _dimensions * 0.5f}
        {
        }

        AABB(const AABB &other)
            : Geometry{other}, _a{other._a}, _b{other._b},
              _dimensions{other._dimensions}, _halfExtents{other._halfExtents},
              _center{other._center}
        {
        }

        virtual ~AABB() noexcept
        {
        }

        void setA(const glm::vec3 &a)
        {
            _a = a;
        }

        void setB(const glm::vec3 &b)
        {
            _b = b;
        }

        glm::vec3 a() const
        {
            return _a;
        }

        glm::vec3 b() const
        {
            return _b;
        }

        glm::vec3 center() const
        {
            return _center;
        }

        float dimensionX() const
        {
            return _dimensions.x;
        }

        float dimensionY() const
        {
            return _dimensions.y;
        }

        float dimensionZ() const
        {
            return _dimensions.z;
        }

        glm::vec3 halfExtents() const
        {
            return _halfExtents;
        }

        glm::vec3 dimensions() const
        {
            return _dimensions;
        }

        virtual bool intersect(const glm::vec3& point) const override;

        virtual bool intersect(const class AABB& aabb, Collision& collision) const override;

        virtual bool intersect(const class Sphere& sphere, Collision& collision) const override;

        virtual bool test(const Geometry& other, Collision& collision) const override
        {
            return other.intersect(*this, collision);
        }

    private:
        glm::vec3 _a;
        glm::vec3 _b;
        glm::vec3 _dimensions;
        glm::vec3 _halfExtents;
        glm::vec3 _center;
    };
}