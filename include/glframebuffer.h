#pragma once

#include "glelement.h"
#include "glrenderbuffer.h"

namespace lithium
{
    class FrameBuffer : public Element
    {
    public:
        enum class Mode
        {
            DEFAULT,
            MULTISAMPLED
        };

        FrameBuffer(glm::ivec2 resolution, Mode mode=Mode::DEFAULT, GLuint colorMode=GL_RGB, GLuint colorAttachment=GL_COLOR_ATTACHMENT0);
        virtual ~FrameBuffer() noexcept;

        virtual void bind() override;
        virtual void unbind() override;

        void createRenderBuffer();

        void attach(RenderBuffer* renderBuffer);

        GLuint textureColorBuffer() const { return _textureColorBuffer; };

    private:
        glm::ivec2 _resolution;
        GLuint _textureColorBuffer;
        Mode _mode;
        GLuint _glTextureMode;
        GLuint _colorAttachment;
    };
}