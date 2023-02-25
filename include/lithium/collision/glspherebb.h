#pragma once

#include "glabstractbb.h"

namespace lithium
{
    class SphereBB : public AbstractBB
    {
    public:
        SphereBB(const glm::vec3 &position, float radii) : AbstractBB{position}, _radii{radii}, _radii2{radii * radii}
        {
        }

        SphereBB(const SphereBB &other) : AbstractBB{other}, _radii{other._radii}, _radii2{other._radii2}
        {
        }

        virtual ~SphereBB() noexcept
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

    private:
        float _radii;
        float _radii2;
    };
}