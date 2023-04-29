#include "glscene.h"

lithium::Scene::Scene()
{
    
}

lithium::Scene::~Scene() noexcept
{

}

void lithium::Scene::addObject(std::shared_ptr<lithium::Object> object)
{
    _objects.emplace(object);
}

void lithium::Scene::addEntity(std::shared_ptr<lithium::Entity> entity)
{
    _entities.emplace(entity);
}

void lithium::Scene::activate()
{
    for(auto obj : _objects)
    {
        obj->stage();
    }
    for(auto entity : _entities)
    {
        entity->stage();
    }
    for(auto bound : _geometries)
    {
        bound->setActive(true);
    }
    _active = true;
}

void lithium::Scene::deactivate()
{
    for(auto obj : _objects)
    {
        obj->unstage();
    }
    for(auto entity : _entities)
    {
        entity->unstage();
    }
    for(auto& bound : _geometries)
    {
        bound->setActive(false);
    }
    _active = false;
}

bool lithium::Scene::checkCollision(const lithium::Sphere& sphere, lithium::Collision& collision)
{
    for(auto geom : _geometries)
    {
        if(geom->recursiveTest(sphere, collision))
        {
            return true;
        }
    }
    return false;
}