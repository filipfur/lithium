#pragma once

#include <vector>
#include "glrenderable.h"

namespace lithium
{
    class RenderGroup : public lithium::IRenderable
    {
    public:
        using FilterType = std::function<bool(lithium::Renderable*)>;
        using ContainerType = std::vector<lithium::Renderable*>;

        RenderGroup(const FilterType& filter) : _filter{filter}
        {

        }

        RenderGroup(const FilterType& filter, 
            const ContainerType& renderables)
            : RenderGroup{filter}
        {
            for(auto renderable : renderables)
            {
                filteredPushBack(renderable);
            }
        }

        virtual ~RenderGroup() noexcept
        {
            for(auto renderable : _renderables)
            {
                renderable->unregisterRenderGroup(this);
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

        void pushBack(lithium::Renderable* renderable)
        {
            _renderables.push_back(renderable);
            renderable->registerRenderGroup(this);
        }

        void insertMany(const ContainerType& list)
        {
            _renderables.insert(_renderables.end(), list.begin(), list.end());
        }

        bool filteredPushBack(lithium::Renderable* renderable)
        {
            if(_filter(renderable))
            {
                pushBack(renderable);
                return true;
            }
            return false;
        }

        bool filteredInsertMany(const ContainerType& renderables)
        {
            bool insertedAny{false};
            for(auto renderable : renderables)
            {
                if(_filter(renderable))
                {
                    pushBack(renderable);
                    insertedAny = true;
                }
            }
            return insertedAny;
        }

        void render(lithium::ShaderProgram* shaderProgram)
        {
            for(auto renderable : _renderables)
            {
                renderable->shade(shaderProgram);
                renderable->draw();
            }
        }
        
        virtual void onRenderableRemoved(Renderable* renderable)
        {
            _renderables.erase(std::find(_renderables.begin(), _renderables.end(), renderable));
        }

    private:
        FilterType _filter;
        ContainerType _renderables;
    };
}