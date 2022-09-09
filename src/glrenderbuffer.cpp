#include "glrenderbuffer.h"

mygl::RenderBuffer::RenderBuffer(int width, int height, bool multisampling) : _multisampling{multisampling}
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

mygl::RenderBuffer::~RenderBuffer() noexcept
{
    glDeleteRenderbuffers(1, &_id);
}

void mygl::RenderBuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, _id);
}

void mygl::RenderBuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}