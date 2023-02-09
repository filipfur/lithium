#pragma once

#include <vector>
#include "glskinnedobject.h"
#include "glupdateable.h"
#include "glrendergroup.h"

namespace lithium
{
    class RenderPipeline
    {
    public:
        RenderPipeline(const glm::ivec2& resolution) : _resolution{resolution}
        {

        }

        lithium::RenderGroup* createRenderGroup(const lithium::RenderGroup::ContainerType& renderables)
        {
            _renderGroups.push_back(lithium::RenderGroup{[](lithium::Renderable* renderable){return false;}, renderables});
            return &_renderGroups.at(_renderGroups.size() - 1);
        }

        lithium::RenderGroup* createRenderGroup(const lithium::RenderGroup::FilterType& filter)
        {
            _renderGroups.push_back(lithium::RenderGroup{filter});
            return &_renderGroups.at(_renderGroups.size() - 1);
        }
        
        virtual void render() = 0;

        void setViewportToResolution()
        {
            glViewport(0, 0, _resolution.x, _resolution.y);
        }

        void addRenderable(lithium::Renderable* renderable)
        {
            for(auto it = _renderGroups.begin(); it != _renderGroups.end(); ++it)
            {
                it->filteredPushBack(renderable);
            }
        }

        void addRenderables(const std::initializer_list<lithium::Renderable*>& renderables)
        {
            for(auto renderable : renderables)
            {
                addRenderable(renderable);
            }
        }

    protected:
        glm::ivec2 _resolution;
        std::vector<lithium::RenderGroup> _renderGroups;
    };
}