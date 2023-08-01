#pragma once

#include <map>
#include <memory>
#include "glelement.h"
#include "gltexture.h"
#include "glrenderbuffer.h"
#include "gltexture.h"

namespace lithium
{
    class FrameBuffer : public Element
    {
    public:

        FrameBuffer(const glm::ivec2& resolution);
        virtual ~FrameBuffer() noexcept;

        virtual void bind() override;

        virtual void unbind() override;

        void createRenderBuffer(GLenum internalFormat=GL_DEPTH24_STENCIL8, GLenum attachment=GL_DEPTH_STENCIL_ATTACHMENT, bool multisampled=false);

        void attach(std::shared_ptr<RenderBuffer> renderBuffer, GLenum attachment);

        std::shared_ptr<lithium::Texture<unsigned char>> createTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0, GLuint internalFormat=GL_RGB, GLuint format=GL_RGB, GLuint type=GL_UNSIGNED_BYTE, GLenum texTarget=GL_TEXTURE_2D);

        void bindTexture(GLuint colorAttachment=GL_COLOR_ATTACHMENT0, GLuint textureUnit=GL_TEXTURE0);

        void declareBuffers();

        void bindAsReadBuffer();

        void bindAsDrawBuffer();

        void checkStatus();

        std::shared_ptr<lithium::Texture<unsigned char>> texture(GLuint colorAttachment)
        {
            return _textures.at(colorAttachment);
        }

        glm::ivec2 resolution() const
        {
            return _resolution;
        }

        std::shared_ptr<lithium::RenderBuffer> renderBuffer() const
        {
            return _renderBuffer;
        }

        void blit(std::shared_ptr<lithium::FrameBuffer> frameBuffer, GLuint fromComponment, GLuint toComponment,
    GLbitfield mask=GL_COLOR_BUFFER_BIT, GLenum filter=GL_NEAREST);

        void blit(std::shared_ptr<lithium::FrameBuffer> frameBuffer, const glm::ivec2& resolution, GLuint fromComponment, GLuint toComponment,
    GLbitfield mask=GL_COLOR_BUFFER_BIT, GLenum filter=GL_NEAREST);

    private:
        static FrameBuffer* _bound;
        glm::ivec2 _resolution;
        GLuint _colorAttachment;
        std::map<GLuint, std::shared_ptr<lithium::Texture<unsigned char>>> _textures;
        std::shared_ptr<lithium::RenderBuffer> _renderBuffer;
    };
}