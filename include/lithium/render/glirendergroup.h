#pragma once

namespace lithium
{
    class Renderable;

    class IRenderGroup
    {
    public:
        virtual void remove(Renderable* renderable) = 0;

        virtual void add(Renderable* renderable) = 0;
    };
}