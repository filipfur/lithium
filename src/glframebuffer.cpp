#include "glframebuffer.h"
#include <iostream>
#include <vector>

#include "gltexture.h"

lithium::FrameBuffer::FrameBuffer(const glm::ivec2& resolution) 
    : _resolution{resolution}
{
    glGenFramebuffers(1, &_id);
}

lithium::FrameBuffer::~FrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &_id);
}

LITHIUM_GLELEMENT_DEF(lithium::FrameBuffer, glBindFramebuffer, GL_FRAMEBUFFER)

void lithium::FrameBuffer::attach(RenderBuffer* renderBuffer, GLenum attachment)
{
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer->id());
    checkStatus();
}

void lithium::FrameBuffer::createRenderBuffer(GLenum internalFormat, GLenum attachment, bool multisampled)
{
    attach(new lithium::RenderBuffer(_resolution, multisampled, internalFormat), attachment);
}

void lithium::FrameBuffer::bindAsReadBuffer()
{
    glBindFramebuffer( GL_READ_FRAMEBUFFER, id() );
}

void lithium::FrameBuffer::bindAsDrawBuffer()
{
    glBindFramebuffer( GL_DRAW_FRAMEBUFFER, id() );
}

void lithium::FrameBuffer::blit(std::shared_ptr<lithium::FrameBuffer> toFrameBuffer,
    GLuint fromComponment, GLuint toComponment,
    GLbitfield mask, GLenum filter)
{
    blit(toFrameBuffer, _resolution, toComponment, fromComponment, mask, filter);
}

void lithium::FrameBuffer::blit(std::shared_ptr<lithium::FrameBuffer> toFrameBuffer, const glm::ivec2& resolution,
    GLuint fromComponment, GLuint toComponment,
    GLbitfield mask, GLenum filter)
{
    bindAsReadBuffer();
    toFrameBuffer->bindAsDrawBuffer();
    glReadBuffer( fromComponment );
    glDrawBuffer( toComponment );
    glBlitFramebuffer( 0, 0, resolution.x, resolution.y, 0, 0, resolution.x, resolution.y, mask, filter );
}

void lithium::FrameBuffer::bindTexture(GLuint colorAttachment, GLuint textureUnit)
{
    auto it = _textures.find(colorAttachment);
    if(it != _textures.end())
    {
        //glBindTexture(it->second->textureMode(), it->second->id());
        it->second->bind(textureUnit);
    }
    else
    {
        std::cerr << "No texture id found for color attachment: " << colorAttachment << std::endl;
    }
}

std::shared_ptr<lithium::Texture<unsigned char>> lithium::FrameBuffer::createTexture(GLuint colorAttachment, GLuint internalFormat, GLuint format, GLuint type, GLenum texTarget)
{
    GLuint samples{4};
    std::shared_ptr<Texture<unsigned char>> tex = std::make_shared<Texture<unsigned char>>(nullptr, _resolution.x, _resolution.y,
        type, internalFormat, format, texTarget, samples);
    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, texTarget, tex->id(), 0);
    checkStatus();
    _textures[colorAttachment] = tex;
    return tex;
}

void lithium::FrameBuffer::declareBuffers()
{
    std::vector<GLuint> attachments;
    attachments.reserve(_textures.size());
    for(auto it=_textures.begin(); it != _textures.end(); ++it)
    {
        attachments.push_back(it->first);
    }
    glDrawBuffers(attachments.size(), attachments.data());
    checkStatus();
}


const char* getFramebufferStatusString(GLenum status) {
    switch (status) {
        case GL_FRAMEBUFFER_UNDEFINED:
            return "GL_FRAMEBUFFER_UNDEFINED: The framebuffer object is not complete because it has no defined buffers.";
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: The framebuffer object is not complete because at least one of its buffer attachments is incomplete.";
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: The framebuffer object is not complete because it has no attached buffers.";
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: The framebuffer object is not complete because the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_DRAW_BUFFERi.";
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: The framebuffer object is not complete because the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER.";
        case GL_FRAMEBUFFER_UNSUPPORTED:
            return "GL_FRAMEBUFFER_UNSUPPORTED: The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.";
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: The value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES, and at least one of the attachments is a texture.";
        default:
            return "Unknown framebuffer status";
    }
}

void lithium::FrameBuffer::checkStatus()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Error: FBO " << getFramebufferStatusString(status) << std::endl;
    }
}