#pragma once

#include "glgeometry.h"

namespace lithium
{
    class Sphere : public Geometry
    {
    public:
        Sphere(const glm::vec3 &position, float radii) : Geometry{position}, _radii{radii}, _radii2{radii * radii}
        {
        }

        Sphere(const Sphere &other) : Geometry{other}, _radii{other._radii}, _radii2{other._radii2}
        {
        }

        virtual ~Sphere() noexcept
        {
        }

        void setRadii(float radii)
        {
            _radii = radii;
            _radii2 = radii * radii;
        }

        float radii2() const
        {
            return _radii2;
        }

        float radii() const
        {
            return _radii;
        }

        virtual bool intersect(const glm::vec3& point) const override;

        virtual bool intersect(const class AABB& aabb, Collision& collision) const override;

        virtual bool intersect(const class Sphere& sphere, Collision& collision) const override;

        virtual bool test(const Geometry& other, Collision& collision) const override
        {
            return other.intersect(*this, collision);
        }

    private:
        float _radii;
        float _radii2;
    };
}