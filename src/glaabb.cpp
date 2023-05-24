#include "glaabb.h"
#include "glsphere.h"

bool lithium::AABB::intersect(const glm::vec3& point) const
{
    glm::vec3 aabbMin = center() - halfExtents();
    glm::vec3 aabbMax = center() + halfExtents();
    return point.x >= aabbMin.x && point.x <= aabbMax.x &&
            point.y >= aabbMin.y && point.y <= aabbMax.y &&
            point.z >= aabbMin.z && point.z <= aabbMax.z;
}

bool lithium::AABB::intersect(const class AABB& aabb, Collision& collision) const
{
    glm::vec3 aabbAMin = center() - halfExtents();
    glm::vec3 aabbAMax = center() + halfExtents();
    glm::vec3 aabbBMin = aabb.center() - aabb.halfExtents();
    glm::vec3 aabbBMax = aabb.center() + aabb.halfExtents();

    bool collided = aabbAMin.x <= aabbBMax.x && aabbAMax.x >= aabbBMin.x &&
            aabbAMin.y <= aabbBMax.y && aabbAMax.y >= aabbBMin.y &&
            aabbAMin.z <= aabbBMax.z && aabbAMax.z >= aabbBMin.z;

    if(collided)
    {
        auto u = glm::normalize(aabb.center() - center());
        glm::vec3 d = center() - aabb.center();
        glm::vec3 dabs = glm::abs(d);
        glm::vec3 overlap = halfExtents() + aabb.halfExtents() - dabs;
        float minOverlap = 1000000.0f;
        int n = 0;
        for(int i{0}; i < 3; ++i)
        {
            if(overlap[i] < minOverlap)
            {
                minOverlap = overlap[i];
                n = i;
            }
        }
        collision.normal = glm::vec3{0.0f};
        collision.normal[n] = d[n] > 0 ? 1.0f : -1.0f;
    }
    return collided;
}

bool lithium::AABB::intersect(const class Sphere& sphere, Collision& collision) const
{
    if(sphere.intersect(*this, collision))
    {
        collision.normal = -collision.normal;
        return true;
    }
    return false;
}