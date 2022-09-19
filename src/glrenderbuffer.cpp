#include "glrenderbuffer.h"

lithium::RenderBuffer::RenderBuffer(glm::ivec2 resolution, lithium::RenderBuffer::Mode mode, GLenum internalFormat) : _mode{mode}
{
    glGenRenderbuffers(1, &_id);
    bind();
    switch(mode)
    {
    case lithium::RenderBuffer::Mode::DEFAULT:
        glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, resolution.x, resolution.y);
    case lithium::RenderBuffer::Mode::MULTISAMPLED:
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, internalFormat, resolution.x, resolution.y);
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