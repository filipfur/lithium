#pragma once

#include <memory>
#include <unordered_set>
#include "glshaderprogram.h"
#include "glirenderable.h"

namespace lithium
{
    class Renderable
    {
    public:
        virtual void shade(lithium::ShaderProgram* shaderProgram) = 0;

        virtual void draw() const = 0;

        void render(lithium::ShaderProgram* shaderProgram)
        {
            shade(shaderProgram);
            draw();
        }

        // Constructor that counts number of Renderables created
        Renderable() noexcept
        {
            ++_countRenderables;
        }

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
        
        std::unordered_set<lithium::IRenderable*>::iterator unregisterRenderGroup(lithium::IRenderable* iRenderable)
        {
            iRenderable->onRenderableRemoved(this);
            auto it = _iRenderables.find(iRenderable);
            return _iRenderables.erase(it);
        }

        void unregisterRenderGroups()
        {
            auto it = _iRenderables.begin();
            while(it != _iRenderables.end())
            {
                it = unregisterRenderGroup(*it);
            }
        }

        static int countRenderables()
        {
            return _countRenderables;
        }

    private:
        std::unordered_set<lithium::IRenderable*> _iRenderables;
        static int _countRenderables;
    };
}