#pragma once

namespace lithium
{
    class Renderable;

    class IRenderable
    {
    public:
        virtual void onRenderableRemoved(Renderable* renderable) = 0;
    };
}