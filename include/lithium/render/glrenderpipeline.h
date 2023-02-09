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
            lithium::RenderGroup* renderGroup = new RenderGroup([](lithium::Renderable* renderable){return false;}, renderables);
            _renderGroups.push_back(renderGroup);
            return renderGroup;
        }

        lithium::RenderGroup* createRenderGroup(const lithium::RenderGroup::FilterType& filter)
        {
            lithium::RenderGroup* renderGroup = new RenderGroup(filter);
            _renderGroups.push_back(renderGroup);
            return renderGroup;
        }
        
        virtual void render() = 0;

        void setViewportToResolution()
        {
            glViewport(0, 0, _resolution.x, _resolution.y);
        }

        void addRenderable(lithium::Renderable* renderable)
        {
            for(auto renderGroup : _renderGroups)
            {
                renderGroup->filteredPushBack(renderable);
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
        std::vector<lithium::RenderGroup*> _renderGroups;
    };
}