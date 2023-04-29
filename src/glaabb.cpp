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
    
    collision.normal = glm::vec3(
        std::min(aabbAMax.x - aabbBMin.x, aabbBMax.x - aabbAMin.x),
        std::min(aabbAMax.y - aabbBMin.y, aabbBMax.y - aabbAMin.y),
        std::min(aabbAMax.z - aabbBMin.z, aabbBMax.z - aabbAMin.z)
    );
    return aabbAMin.x <= aabbBMax.x && aabbAMax.x >= aabbBMin.x &&
            aabbAMin.y <= aabbBMax.y && aabbAMax.y >= aabbBMin.y &&
            aabbAMin.z <= aabbBMax.z && aabbAMax.z >= aabbBMin.z;
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