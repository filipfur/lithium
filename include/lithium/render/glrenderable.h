#pragma once

#include <memory>
#include <set>
#include "glshaderprogram.h"

namespace lithium
{
    class Renderable
    {
    public:
        virtual void shade(lithium::ShaderProgram* shaderProgram) const = 0;

        virtual void draw() const = 0;

        void registerRenderGroup(std::shared_ptr<lithium::RenderGroup> renderGroup)
        {
            _renderGroups.emplace(renderGroup);
        }
        
        void unregisterRenderGroup()
        {
            //_renderGroups.dele
        }

    private:
        std::set<std::shared_ptr<lithium::RenderGroup>> _renderGroups;
    };
}