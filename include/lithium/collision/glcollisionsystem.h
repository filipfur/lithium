#pragma once

#include <glm/glm.hpp>

#include "glspherebb.h"
#include "glaabb.h"

namespace lithium
{
    class CollisionSystem
    {
    public:
        CollisionSystem()
        {
        }

        virtual ~CollisionSystem() noexcept
        {
        }

        bool checkCollision2D(const SphereBB &sphereBB, glm::vec2 &normal)
        {
            for (SphereBB &cBB : _sphereBBs)
            {
                if (test2D(cBB, sphereBB, normal))
                {
                    return true;
                }
            }
            for (AABB &rBB : _aabbs)
            {
                if (test2D(rBB, sphereBB, normal))
                {
                    return true;
                }
            }
            return false;
        }

        bool checkCollision3D(const SphereBB &sphereBB, glm::vec3 &normal)
        {
            for (SphereBB &cBB : _sphereBBs)
            {
                if (test3D(cBB, sphereBB, normal))
                {
                    return true;
                }
            }
            for (AABB &rBB : _aabbs)
            {
                if (test3D(sphereBB, rBB, normal))
                {
                    return true;
                }
            }
            return false;
        }

        static bool test2D(const glm::vec3 &p0, const SphereBB &c1, glm::vec2 &n)
        {
            const glm::vec3 delta{c1.position() - p0};
            // n = glm::normalize(glm::vec2{delta.x, delta.z});
            n = glm::vec2(delta.x, delta.z);
            return (delta.x * delta.x + delta.z * delta.z) <= c1.radii2();
        }

        static bool test2D(const SphereBB &c0, const SphereBB &c1, glm::vec2 &n)
        {
            const glm::vec3 delta{c1.position() - c0.position()};
            // n = glm::normalize(glm::vec2{delta.x, delta.z});
            n = glm::vec2(delta.x, delta.z);
            return sqrt(delta.x * delta.x + delta.z * delta.z) <= (c0.radii() + c1.radii());
        }

        static bool test2D(const SphereBB &c0, const AABB &r1, glm::vec2 &n)
        {
            // get center point circle first
            glm::vec2 center(c0.position().x, c0.position().z);
            // calculate AABB info (center, half-extents)
            glm::vec2 aabb_half_extents(r1.dimX() * 0.5f, r1.dimZ() * 0.5f);
            glm::vec2 aabb_center(r1.center().x, r1.center().z);
            // get difference vector between both centers
            glm::vec2 difference = center - aabb_center;
            glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
            // add clamped value to AABB_center and we get the value of box closest to circle
            glm::vec2 closest = aabb_center + clamped;
            // retrieve vector between center circle and closest point AABB and check if length <= radius
            difference = center - closest;
            // n = glm::normalize(difference);
            n = difference;
            return glm::length2(difference) < c0.radii2();
        }

        static bool test2D(const AABB &r0, const SphereBB &c1, glm::vec2 &n)
        {
            return test2D(c1, r0, n);
        }

        static bool test2D(const AABB &r0, const AABB &r1, glm::vec2 &n)
        {
            // collision x-axis?
            bool collisionX = r0.center().x + r0.dimX() >= r1.center().x &&
                              r1.center().x + r1.dimX() >= r0.center().x;
            // collision y-axis?
            bool collisionZ = r0.center().z + r0.dimZ() >= r1.center().z &&
                              r1.center().z + r1.dimZ() >= r0.center().z;
            // collision only if on both axes
            return collisionX && collisionZ;
        }

        static bool test3D(const SphereBB &c0, const SphereBB &c1, glm::vec3& n)
        {
            return glm::distance(c0.position(), c1.position()) <= (c0.radii() + c1.radii());
        }

        static bool test3D(const SphereBB &c0, const AABB &r1, glm::vec3& n)
        {
            /*glm::vec3 test{0.0f};
            for (int i{0}; i < 3; ++i)
            {
                if (c0.position()[i] < (r1.position()[i] + r1.a()[i]))
                {
                    test[i] = r1.position()[i] + r1.a()[i];
                }
                else if (c0.position()[i] > (r1.position()[i] + r1.b()[i]))
                {
                    test[i] = r1.position()[i] + r1.b()[i];
                }
            }

            return glm::distance2(c0.position(), test) <= c0.radii2();*/

            // get center point circle first
            glm::vec3 center(c0.position().x, c0.position().y, c0.position().z);
            // calculate AABB info (center, half-extents)
            glm::vec3 aabb_half_extents(r1.dimX() * 0.5f, r1.dimY() * 0.5f, r1.dimZ() * 0.5f);
            glm::vec3 aabb_center(r1.center().x, r1.center().y, r1.center().z);
            // get difference vector between both centers
            glm::vec3 difference = center - aabb_center;
            glm::vec3 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
            // add clamped value to AABB_center and we get the value of box closest to circle
            glm::vec3 closest = aabb_center + clamped;
            // retrieve vector between center circle and closest point AABB and check if length <= radius
            difference = center - closest;
            // n = glm::normalize(difference);
            n = difference;
            return glm::length2(difference) < c0.radii2();
        }

        void insertSphereBB(const SphereBB &sphereBB)
        {
            _sphereBBs.push_back(sphereBB);
        }

        void insertAABB(const AABB &aabb)
        {
            _aabbs.push_back(aabb);
        }

    private:
        std::vector<SphereBB> _sphereBBs;
        std::vector<AABB> _aabbs;
    };
}