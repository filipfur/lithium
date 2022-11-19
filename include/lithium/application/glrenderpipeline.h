#pragma once

#include <vector>
#include "glupdateable.h"
#include "glmodel.h"

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

        void insertObjects(const std::initializer_list<lithium::Object*>& list)
        {
            _objects.insert(_objects.end(), list);
        }

        void insertModel(lithium::Model* model)
        {
            _models.push_back(model);
        }

        void insertModels(const std::initializer_list<lithium::Model*>& list)
        {
            _models.insert(_models.end(), list);
        }

        void removeObject(lithium::Object* object)
        {
            _objects.erase(std::remove(_objects.begin(), _objects.end(), object), _objects.end());
        }

        void removeModel(lithium::Model* model)
        {
            _models.erase(std::remove(_models.begin(), _models.end(), model), _models.end());
        }

        void setViewportToResolution()
        {
            glViewport(0, 0, _resolution.x, _resolution.y);
        }

    protected:
        glm::ivec2 _resolution;
        std::vector<lithium::Object*> _objects;
        std::vector<lithium::Model*> _models;
    };
}