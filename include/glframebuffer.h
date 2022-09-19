#pragma once

#include "glelement.h"
#include "gltexture.h"
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

        void createRenderBuffer(lithium::RenderBuffer::Mode mode, GLenum internalFormat=GL_DEPTH24_STENCIL8, GLenum attachment=GL_DEPTH_STENCIL_ATTACHMENT);

        void attach(RenderBuffer* renderBuffer, GLenum attachment);

        void createTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0, GLuint internalFormat=GL_RGB, GLuint format=GL_RGB, GLuint type=GL_UNSIGNED_BYTE, GLuint filter=GL_NEAREST, GLuint wrap=GL_CLAMP_TO_EDGE);

        void bindTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0);

        void declareBuffers();

    private:
        static FrameBuffer* _bound;
        glm::ivec2 _resolution;
        Mode _mode;
        GLuint _glTextureMode;
        GLuint _colorAttachment;
        std::map<GLuint, GLuint> _textureIds;
    };
}