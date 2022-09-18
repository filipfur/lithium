#include "glframebuffer.h"
#include <iostream>

lithium::FrameBuffer* lithium::FrameBuffer::_bound{nullptr};

lithium::FrameBuffer::FrameBuffer(glm::ivec2 resolution, lithium::FrameBuffer::Mode mode) 
    : _resolution{resolution}, _mode{mode}
{
    glGenFramebuffers(1, &_id);
}

lithium::FrameBuffer::~FrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &_id);
}

void lithium::FrameBuffer::bind()
{
    if(_bound != this)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        _bound = this;
    }
}

void lithium::FrameBuffer::unbind()
{
    _bound = nullptr;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void lithium::FrameBuffer::attach(RenderBuffer* renderBuffer)
{
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer->id());
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to bind framebuffer!" << std::endl;
    }
    unbind();
}

void lithium::FrameBuffer::createRenderBuffer()
{
    attach(new lithium::RenderBuffer(_resolution,
        _mode == lithium::FrameBuffer::Mode::DEFAULT 
            ? lithium::RenderBuffer::Mode::DEFAULT 
            : lithium::RenderBuffer::Mode::MULTISAMPLED));
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

void lithium::FrameBuffer::createTexture(GLuint colorAttachment, GLuint colorMode)
{
    bind();
    GLuint texId;
    glGenTextures(1, &texId);
    switch(_mode)
    {
    case lithium::FrameBuffer::Mode::DEFAULT:
        _glTextureMode = GL_TEXTURE_2D;
        glBindTexture(_glTextureMode, texId);
        glTexImage2D(_glTextureMode, 0, colorMode, _resolution.x, _resolution.y, 0, colorMode, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        break;
    case lithium::FrameBuffer::Mode::MULTISAMPLED:
        _glTextureMode = GL_TEXTURE_2D_MULTISAMPLE;
        glBindTexture(_glTextureMode, texId);
        glTexImage2DMultisample(_glTextureMode, 4, colorMode, _resolution.x, _resolution.y, GL_TRUE);
        break;
    }
    glBindTexture(_glTextureMode, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, _glTextureMode, texId, 0);
    unbind();
    _textureIds[colorAttachment] = texId;
}