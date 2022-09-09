#include "glframebuffer.h"
#include <iostream>

mygl::FrameBuffer::FrameBuffer(int width, int height, bool multisampling, GLuint colorMode) : _multisampling{multisampling}
{
    glGenFramebuffers(1, &_id);
    bind();
    GLuint mod = multisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    glGenTextures(1, &_textureColorBuffer);
    glBindTexture(mod, _textureColorBuffer);
    if(multisampling)
    {
        glTexImage2DMultisample(mod, 4, colorMode, width, height, GL_TRUE);
    }
    else
    {
        glTexImage2D(mod, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    }

    glBindTexture(mod, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mod, _textureColorBuffer, 0);
    unbind();
}

mygl::FrameBuffer::~FrameBuffer() noexcept
{
    glDeleteFramebuffers(1, &_id);
}

void mygl::FrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
}

void mygl::FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void mygl::FrameBuffer::attach(RenderBuffer* renderBuffer)
{
    bind();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer->id());
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to bind framebuffer!" << std::endl;
    }
    unbind();
}