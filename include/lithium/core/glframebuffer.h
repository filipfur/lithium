#pragma once

#include "glelement.h"
#include "gltexture.h"
#include "glrenderbuffer.h"
#include <map>
#include "gltexture.h"

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

        FrameBuffer(const glm::ivec2& resolution, Mode mode=Mode::DEFAULT);
        virtual ~FrameBuffer() noexcept;

        virtual void bind() override;

        virtual void unbind() override;

        void createRenderBuffer(lithium::RenderBuffer::Mode mode, GLenum internalFormat=GL_DEPTH24_STENCIL8, GLenum attachment=GL_DEPTH_STENCIL_ATTACHMENT);

        void attach(RenderBuffer* renderBuffer, GLenum attachment);

        void createTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0, GLuint internalFormat=GL_RGB, GLuint format=GL_RGB, GLuint type=GL_UNSIGNED_BYTE);

        void bindTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0, GLuint textureUnit=GL_TEXTURE0);

        void declareBuffers();

        void bindAsReadBuffer();

        void bindAsDrawBuffer();

        std::shared_ptr<lithium::Texture<unsigned char>> texture(GLuint colorAttachment)
        {
            return _textures.at(colorAttachment);
        }

        void blit(std::shared_ptr<lithium::FrameBuffer> frameBuffer, GLuint fromComponment, GLuint toComponment,
    GLbitfield mask=GL_COLOR_BUFFER_BIT, GLenum filter=GL_NEAREST);

        void blit(std::shared_ptr<lithium::FrameBuffer> frameBuffer, const glm::ivec2& resolution, GLuint fromComponment, GLuint toComponment,
    GLbitfield mask=GL_COLOR_BUFFER_BIT, GLenum filter=GL_NEAREST);

    private:
        static FrameBuffer* _bound;
        glm::ivec2 _resolution;
        Mode _mode;
        GLuint _colorAttachment;
        std::map<GLuint, std::shared_ptr<lithium::Texture<unsigned char>>> _textures;
    };
}