#pragma once

#include "glelement.h"
#include "glrenderbuffer.h"
#include <map>

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

        FrameBuffer(glm::ivec2 resolution, Mode mode=Mode::DEFAULT);
        virtual ~FrameBuffer() noexcept;

        virtual void bind() override;
        virtual void unbind() override;

        void createRenderBuffer();

        void attach(RenderBuffer* renderBuffer);

        void createTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0, GLuint colorMode=GL_RGB);

        void bindTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0);

    private:
        static FrameBuffer* _bound;
        glm::ivec2 _resolution;
        Mode _mode;
        GLuint _glTextureMode;
        GLuint _colorAttachment;
        std::map<GLuint, GLuint> _textureIds;
    };
}