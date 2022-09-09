#pragma once

#include "glelement.h"

namespace mygl
{
    class RenderBuffer : public Element
    {
    public:
        RenderBuffer(int width, int height, bool multisampling);
        virtual ~RenderBuffer() noexcept;

        virtual void bind() override;
        virtual void unbind() override;

    private:
        bool _multisampling;
    };
}