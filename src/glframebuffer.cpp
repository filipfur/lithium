#include "glframebuffer.h"
#include <iostream>
#include <vector>

lithium::FrameBuffer::FrameBuffer(glm::ivec2 resolution, lithium::FrameBuffer::Mode mode) 
    : _resolution{resolution}, _mode{mode}
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
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to bind framebuffer!" << std::endl;
    }
}

void lithium::FrameBuffer::createRenderBuffer(lithium::RenderBuffer::Mode mode, GLenum internalFormat, GLenum attachment)
{
    attach(new lithium::RenderBuffer(_resolution, mode, internalFormat), attachment);
}

void lithium::FrameBuffer::bindTexture(GLuint colorAttachment)
{
    auto it = _textureIds.find(colorAttachment);
    if(it != _textureIds.end())
    {
        glBindTexture(_glTextureMode, it->second);
    }
    else
    {
        std::cerr << "No texture id found for color attachment: " << colorAttachment << std::endl;
    }
}

void lithium::FrameBuffer::createTexture(GLuint colorAttachment, GLuint internalFormat, GLuint format, GLuint type, GLuint filter, GLuint wrap)
{
    GLuint texId;
    glGenTextures(1, &texId);
    switch(_mode)
    {
    case lithium::FrameBuffer::Mode::DEFAULT:
        _glTextureMode = GL_TEXTURE_2D;
        glBindTexture(_glTextureMode, texId);
        glTexImage2D(_glTextureMode, 0, internalFormat, _resolution.x, _resolution.y, 0, format, type, NULL);
        break;
    case lithium::FrameBuffer::Mode::MULTISAMPLED:
        _glTextureMode = GL_TEXTURE_2D_MULTISAMPLE;
        glBindTexture(_glTextureMode, texId);
        glTexImage2DMultisample(_glTextureMode, 4, internalFormat, _resolution.x, _resolution.y, GL_TRUE);
        break;
    }
    glTexParameteri(_glTextureMode, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(_glTextureMode, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(_glTextureMode, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(_glTextureMode, GL_TEXTURE_WRAP_T, wrap);

    glBindTexture(_glTextureMode, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, _glTextureMode, texId, 0);
    _textureIds[colorAttachment] = texId;
}

void lithium::FrameBuffer::declareBuffers()
{
    std::vector<GLuint> attachments;
    attachments.reserve(_textureIds.size());
    for(auto it=_textureIds.begin(); it != _textureIds.end(); ++it)
    {
        attachments.push_back(it->first);
    }
    glDrawBuffers(attachments.size(), attachments.data());
}