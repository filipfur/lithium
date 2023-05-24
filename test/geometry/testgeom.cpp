#include <iostream>
#include "glaabb.h"
#include "glsphere.h"

int main(int argc, const char* argv[])
{
    lithium::Collision collision;
    auto a = new lithium::AABB(glm::vec3(0.0f), glm::vec3(-1.0f), glm::vec3(1.0f));
    auto b = new lithium::AABB(glm::vec3(1.0f), glm::vec3(-1.0f), glm::vec3(1.0f));
    auto c = new lithium::AABB(glm::vec3(2.1f), glm::vec3(-1.0f), glm::vec3(1.0f));
    auto s = new lithium::Sphere(glm::vec3(0.0f), 1.0f);
    auto world = new lithium::Sphere(glm::vec3(20.0f), 100.0f);

    world->addChild(a);

    assert(a->intersect(*b, collision));
    assert(b->intersect(*a, collision));
    assert(!a->intersect(*c, collision));

    assert(s->intersect(*a, collision));
    assert(s->intersect(*b, collision));
    assert(!s->intersect(*c, collision));

    std::vector<lithium::Geometry*> geoms;
    geoms.push_back(a);
    geoms.push_back(b);
    geoms.push_back(c);
    geoms.push_back(s);
    geoms.push_back(world);

    assert(geoms[0]->test(*geoms[1], collision));
    assert(!geoms[0]->test(*geoms[2], collision));

    assert(geoms[4]->recursiveTest(*geoms[3], collision));

    std::cout << "All tests passed!" << std::endl;

    return 0;
}