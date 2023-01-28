#pragma once

#include <vector>
#include "glskinnedobject.h"
#include "glupdateable.h"

namespace lithium
{
    class RenderPipeline : public lithium::Updateable
    {
    public:
        RenderPipeline(const glm::ivec2& resolution) : _resolution{resolution}
        {

        }

        virtual void update(float dt) override = 0;

        virtual void render() = 0;

        void insertObject(lithium::Object* object)
        {
            _objects.push_back(object);
        }

        void insertObjects(const std::vector<lithium::Object*>& list)
        {
            _objects.insert(_objects.end(), list.begin(), list.end());
        }

        void insertSkinnedObject(lithium::SkinnedObject* object)
        {
            _skinnedObjects.push_back(object);
        }

        void insertSkinnedObjects(const std::vector<lithium::SkinnedObject*>& list)
        {
            _skinnedObjects.insert(_skinnedObjects.end(), list.begin(), list.end());
        }

        void removeObject(lithium::Object* object)
        {
            _objects.erase(std::remove(_objects.begin(), _objects.end(), object), _objects.end());
        }

        void removeSkinnedObject(lithium::SkinnedObject* object)
        {
            _skinnedObjects.erase(std::remove(_skinnedObjects.begin(), _skinnedObjects.end(), object), _skinnedObjects.end());
        }

        void setViewportToResolution()
        {
            glViewport(0, 0, _resolution.x, _resolution.y);
        }

    protected:
        glm::ivec2 _resolution;
        std::vector<lithium::Object*> _objects;
        std::vector<lithium::SkinnedObject*> _skinnedObjects;
    };
}