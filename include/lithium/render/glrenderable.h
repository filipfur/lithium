#pragma once

#include <memory>
#include <set>
#include "glshaderprogram.h"
#include "glirenderable.h"

namespace lithium
{
    class Renderable
    {
    public:
        virtual void shade(lithium::ShaderProgram* shaderProgram) = 0;

        virtual void draw() const = 0;

        virtual ~Renderable() noexcept
        {
            for(auto iRenderable : _iRenderables)
            {
                iRenderable->onRenderableRemoved(this);
            }
            _iRenderables.clear();
        }

        void registerRenderGroup(lithium::IRenderable* iRenderable)
        {
            _iRenderables.emplace(iRenderable);
        }
        
        void unregisterRenderGroup(lithium::IRenderable* iRenderable)
        {
            _iRenderables.erase(iRenderable);
        }

    private:
        std::set<lithium::IRenderable*> _iRenderables;
    };
}