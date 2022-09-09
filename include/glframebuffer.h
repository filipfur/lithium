#pragma once

#include "glelement.h"
#include "glrenderbuffer.h"

namespace mygl
{
    class FrameBuffer : public Element
    {
    public:
        FrameBuffer(int width, int height, bool multisampling, GLuint colorMode=GL_RGB);
        virtual ~FrameBuffer() noexcept;

        virtual void bind() override;
        virtual void unbind() override;

        void attach(RenderBuffer* renderBuffer);

        GLuint textureColorBuffer() const { return _textureColorBuffer; };

    private:
        GLuint _textureColorBuffer;
        bool _multisampling;
    };
}