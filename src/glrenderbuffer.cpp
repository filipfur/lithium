#include "glrenderbuffer.h"

lithium::RenderBuffer::RenderBuffer(int width, int height, bool multisampling) : _multisampling{multisampling}
{
    glGenRenderbuffers(1, &_id);
    bind();
    if(multisampling)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
    unbind();
}

lithium::RenderBuffer::~RenderBuffer() noexcept
{
    glDeleteRenderbuffers(1, &_id);
}

void lithium::RenderBuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
}

void lithium::RenderBuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}