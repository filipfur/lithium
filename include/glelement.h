#pragma once

#include <glad/glad.h>

namespace mygl
{
    class Element
    {
    public:
        Element() {};
        Element(GLuint id) : _id{id} {}
        virtual ~Element() noexcept
        {

        }

        GLuint id() const { return _id; }

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        GLuint _id{0u};
    };
}