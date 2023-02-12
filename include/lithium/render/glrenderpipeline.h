#pragma once

#include <vector>
#include "glskinnedobject.h"
#include "glupdateable.h"
#include "glrendergroup.h"
#include "glrenderstage.h"
#include <memory>

namespace lithium
{
    class RenderPipeline
    {
    public:
        RenderPipeline(const glm::ivec2& resolution) : _resolution{resolution}
        {

        }

        std::shared_ptr<lithium::RenderGroup> createRenderGroup(const lithium::RenderGroup::ContainerType& renderables)
        {
            std::shared_ptr<lithium::RenderGroup> renderGroup = std::make_shared<RenderGroup>([](lithium::Renderable* renderable){return false;}, renderables);
            _renderGroups.push_back(renderGroup);
            return renderGroup;
        }

        std::shared_ptr<lithium::RenderGroup> createRenderGroup(const lithium::RenderGroup::FilterType& filter)
        {
            std::shared_ptr<lithium::RenderGroup> renderGroup = std::make_shared<RenderGroup>(filter);
            _renderGroups.push_back(renderGroup);
            return renderGroup;
        }

        std::shared_ptr<lithium::RenderStage> addRenderStage(std::shared_ptr<lithium::RenderStage> renderStage)
        {
            _renderStages.push_back(renderStage);
            return renderStage;
        }
        
        virtual void render()
        {
            for(auto it = _renderStages.begin(); it != _renderStages.end(); ++it)
            {
                (*it)->render();
            }
        }

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
        std::vector<std::shared_ptr<lithium::RenderGroup>> _renderGroups;
        std::vector<std::shared_ptr<lithium::RenderStage>> _renderStages;
    };
}