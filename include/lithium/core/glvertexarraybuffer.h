#pragma once

#include "glbuffer.h"

namespace lithium
{
    class VertexArrayBuffer : public Buffer<GLfloat, GL_ARRAY_BUFFER>
    {
    public:
        VertexArrayBuffer()
        {

        }

        VertexArrayBuffer(const VertexArrayBuffer& other) : Buffer{other}
        {

        }
        
        virtual VertexArrayBuffer* clone() const override
        {
            return new VertexArrayBuffer(*this);
        }

        virtual ~VertexArrayBuffer() noexcept
        {

        }

    private:

    };
}