#include "glrenderbuffer.h"

lithium::RenderBuffer::RenderBuffer(glm::ivec2 resolution, bool multisampled, GLenum internalFormat)
{
    glGenRenderbuffers(1, &_id);
    bind();
    if(multisampled)
    {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, internalFormat, resolution.x, resolution.y);
    }
    else
    {
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, resolution.x, resolution.y);
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