#pragma once

#include <vector>
#include <unordered_set>
#include "glrenderable.h"

namespace lithium
{
    class RenderGroup : public lithium::IRenderGroup
    {
    public:
        using FilterType = std::function<bool(lithium::Renderable*)>;
        using ContainerType = std::set<lithium::Renderable*>;

        RenderGroup(const FilterType& filter) : _filter{filter}
        {
            static int nextId{0};
            _id = nextId++;
        }

        virtual ~RenderGroup() noexcept
        {
            auto it = _renderables.begin();
            while(it != _renderables.end())
            {
                (*it)->detach(this);
                it = _renderables.begin();
            }
            _renderables.clear();
        }

        void forEach(const std::function<void(lithium::Renderable*)>& callback)
        {
            for(auto renderable : _renderables)
            {
                callback(renderable);
            }
        }

        virtual void add(lithium::Renderable* renderable) override
        {
            _renderables.emplace(renderable);
        }

        bool filter(lithium::Renderable* renderable)
        {
            return _filter(renderable);
        }

        void render(lithium::ShaderProgram* shaderProgram)
        {
            for(auto renderable : _renderables)
            {
                renderable->shade(shaderProgram);
                renderable->draw();
            }
        }

        void render(std::shared_ptr<lithium::ShaderProgram> shaderProgram)
        {
            render(shaderProgram.get());
        }
        
        virtual void remove(Renderable* renderable) override
        {
            //_renderables.erase(std::find_if(_renderables.begin(), _renderables.end(), renderable));
            _renderables.erase(renderable);
        }

        size_t count() const
        {
            return _renderables.size();
        }

        int id() const
        {
            return _id;
        }

        ContainerType::iterator begin()
        {
            return _renderables.begin();
        }

        ContainerType::iterator end()
        {
            return _renderables.end();
        }

    private:
        int _id;
        FilterType _filter;
        ContainerType _renderables;
    };
}