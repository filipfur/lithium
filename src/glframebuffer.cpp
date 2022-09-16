#include "glframebuffer.h"
#include <iostream>

lithium::FrameBuffer::FrameBuffer(glm::ivec2 resolution, lithium::FrameBuffer::Mode mode, GLuint colorMode, GLuint colorAttachment) 
    : _resolution{resolution}, _mode{mode}, _colorAttachment{colorAttachment}
{
    glGenFramebuffers(1, &_id);
    bind();
    glGenTextures(1, &_textureColorBuffer);
    switch(mode)
    {
    case lithium::FrameBuffer::Mode::DEFAULT:
        _glTextureMode = GL_TEXTURE_2D;
        glBindTexture(_glTextureMode, _textureColorBuffer);
        glTexImage2D(_glTextureMode, 0, colorMode, resolution.x, resolution.y, 0, colorMode, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        break;
    case lithium::FrameBuffer::Mode::MULTISAMPLED:
        _glTextureMode = GL_TEXTURE_2D_MULTISAMPLE;
        glBindTexture(_glTextureMode, _textureColorBuffer);
        glTexImage2DMultisample(_glTextureMode, 4, colorMode, resolution.x, resolution.y, GL_TRUE);
        break;
    }

    glBindTexture(_glTextureMode, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, _glTextureMode, _textureColorBuffer, 0);
    unbind();
}

lithium::FrameBuffer::~FrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &_id);
}

void lithium::FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void lithium::FrameBuffer::unbind()
{
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