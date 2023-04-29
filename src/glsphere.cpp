#include "glsphere.h"
#include "glaabb.h"

bool lithium::Sphere::intersect(const glm::vec3& point) const
{
    return glm::length2(position() - point) <= radii2();
}

bool lithium::Sphere::intersect(const AABB& aabb, Collision& collision) const
{
    // get difference vector between both centers
    glm::vec3 difference = position() - aabb.center();
    glm::vec3 clamped = glm::clamp(difference, -aabb.halfExtents(), aabb.halfExtents());
    // add clamped value to AABB_center and we get the value of box closest to circle
    glm::vec3 closest = aabb.center() + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    collision.normal = position() - closest;
    float dist = glm::length(collision.normal);
    bool c = glm::length2(collision.normal) < radii2();
    // n = glm::normalize(difference);
    return c;
}

bool lithium::Sphere::intersect(const Sphere& sphere, Collision& collision) const
{
    float sumRadii = radii() + sphere.radii();
    collision.normal = position() - sphere.position();
    float dist = glm::length(collision.normal);
    bool c = dist <= sumRadii;
    return c;
}