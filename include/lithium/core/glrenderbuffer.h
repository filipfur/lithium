#pragma once

#include "glelement.h"

#include <glm/glm.hpp>

namespace lithium
{
    class RenderBuffer : public Element
    {
    public:
        RenderBuffer(glm::ivec2 resolution, bool multisampled, GLenum internalFormat=GL_DEPTH24_STENCIL8);
        virtual ~RenderBuffer() noexcept;

        virtual void bind() override;
        virtual void unbind() override;
    };
}