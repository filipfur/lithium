#pragma once

#include <vector>
#include <set>
#include <functional>
#include "glentity.h"
#include "globject.h"
#include "glnode.h"
#include "glinstancedobject.h"
#include "glupdateable.h"
#include "glaabb.h"
#include "glsphere.h"
#include "glrenderpipeline.h"

namespace lithium
{
    class Scene : public lithium::Updateable
    {
    public:
        Scene();

        virtual ~Scene() noexcept;

        void activate();

        void deactivate();

        void addObject(std::shared_ptr<lithium::Object> object);

        void addEntity(std::shared_ptr<lithium::Entity> entity);

        void copyEntities(std::set<ecs::Entity*>& entities)
        {
            for(auto entity : _entities)
            {
                entities.emplace(entity.get());
            }
        }

        virtual void update(float dt)
        {
            lithium::Updateable::update(dt);
            for(auto obj : _objects)
            {
                obj->update(dt);
            }
        }

        bool active() const
        {
            return _active;
        }

        void removeEntity(std::shared_ptr<lithium::Entity> entity)
        {
            //TODO: Also remove from GameState ...
            if(_active)
            {
                entity->detachAll();
            }
            _entities.erase(entity);
        }

        void removeObject(std::shared_ptr<lithium::Object> object)
        {
            if(_active)
            {
                object->detachAll();
            }
            _objects.erase(object);
        }

        std::shared_ptr<lithium::Object> object(size_t index)
        {  
            auto it = _objects.begin();
            std::advance(it, index);
            return it == _objects.end() ? nullptr : *it;
        }

        void forEachObject(std::function<void(lithium::Object*)> func)
        {
            for(auto obj : _objects)
            {
                func(obj.get());
            }
        }

        void forEachEntity(std::function<void(lithium::Entity*)> func)
        {
            for(auto entity : _entities)
            {
                func(entity.get());
            }
        }

        void attach(std::shared_ptr<lithium::RenderPipeline> pipeline)
        {
            for(auto obj : _objects)
            {
                pipeline->attach(obj.get());
            }
            for(auto entity : _entities)
            {
                pipeline->attach(entity.get());
            }
        }

        void addGeometry(lithium::Geometry* geometry)
        {
            _geometries.push_back(geometry);
        }

        lithium::Geometry* geometry(size_t index)
        {
            return _geometries.at(index);
        }

        size_t geometryCount() const
        {
            return _geometries.size();
        }

        bool checkCollision(const lithium::Sphere& sphere, lithium::Collision& collision);

    private:
        std::vector<lithium::Geometry*> _geometries;
        std::set<std::shared_ptr<lithium::Object>> _objects;
        std::set<std::shared_ptr<lithium::Entity>> _entities;
        bool _active{false};
    };
}