#pragma once

#include "glelement.h"

#include <glm/glm.hpp>

namespace lithium
{
    class RenderBuffer : public Element
    {
    public:
        enum class Mode
        {
            DEFAULT,
            MULTISAMPLED
        };

        RenderBuffer(glm::ivec2 resolution, Mode mode=Mode::DEFAULT);
        virtual ~RenderBuffer() noexcept;

        virtual void bind() override;
        virtual void unbind() override;

    private:
        Mode _mode;
    };
}