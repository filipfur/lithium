#pragma once

#include <memory>
#include <unordered_set>
#include "glshaderprogram.h"
#include "glirendergroup.h"

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
        Renderable()
        {
            ++_countRenderables;
        }

        virtual ~Renderable() noexcept
        {
            for(auto iRenderable : _iRenderables)
            {
                iRenderable->remove(this);
            }
            _iRenderables.clear();
        }

        void attach(lithium::IRenderGroup* iRenderable)
        {
            _iRenderables.emplace(iRenderable);
        }
        
        std::unordered_set<lithium::IRenderGroup*>::iterator detach(lithium::IRenderGroup* iRenderable)
        {
            iRenderable->remove(this);
            auto it = _iRenderables.find(iRenderable);
            return _iRenderables.erase(it);
        }

        bool isAttached(lithium::IRenderGroup* iRenderable)
        {
            return _iRenderables.find(iRenderable) != _iRenderables.end();
        }

        bool isAttached(std::shared_ptr<lithium::IRenderGroup> iRenderable)
        {
            return isAttached(iRenderable.get());
        }

        bool hasAttachments()
        {
            return !_iRenderables.empty();
        }

        int numAttachments()
        {
            return _iRenderables.size();
        }

        void detachAll()
        {
            auto it = _iRenderables.begin();
            while(it != _iRenderables.end())
            {
                it = detach(*it);
            }
        }

        void stage()
        {
            for(auto iRenderable : _iRenderables)
            {
                iRenderable->add(this);
            }
        }

        void unstage()
        {
            for(auto iRenderable : _iRenderables)
            {
                iRenderable->remove(this);
            }
        }

        static int countRenderables()
        {
            return _countRenderables;
        }

        void setGroupId(int groupId)
        {
            _groupId = groupId;
        }

        int groupId() const
        {
            return _groupId;
        }

    private:
        int _groupId{0};
        std::unordered_set<lithium::IRenderGroup*> _iRenderables;
        static int _countRenderables;
    };
}