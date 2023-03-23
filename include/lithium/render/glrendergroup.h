#pragma once

#include <vector>
#include <unordered_set>
#include "glrenderable.h"

namespace lithium
{
    class RenderGroup : public lithium::IRenderable
    {
    public:
        using FilterType = std::function<bool(lithium::Renderable*)>;
        using ContainerType = std::set<lithium::Renderable*>;

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
            auto it = _renderables.begin();
            while(it != _renderables.end())
            {
                (*it)->unregisterRenderGroup(this);
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

        void pushBack(lithium::Renderable* renderable)
        {
            _renderables.emplace(renderable);
            renderable->registerRenderGroup(this);
        }

        /*void insertMany(const ContainerType& list)
        {
            _renderables.emplace(_renderables.end(), list.begin(), list.end());
        }*/

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
            //_renderables.erase(std::find_if(_renderables.begin(), _renderables.end(), renderable));
            _renderables.erase(renderable);
        }

        size_t count() const
        {
            return _renderables.size();
        }

    private:
        FilterType _filter;
        ContainerType _renderables;
    };
}