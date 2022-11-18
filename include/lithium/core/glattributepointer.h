#pragma once

#include <glad/glad.h>

namespace lithium
{
    template <GLenum T=GL_FLOAT>
    class AttributePointer
    {
    public:
        AttributePointer(GLuint layout, GLuint numComponents, GLsizei stride, void* offset)
            : _layout{layout}, _numComponents{numComponents}, _stride{stride}, _offset{offset}
        {

        }

        virtual ~AttributePointer() noexcept
        {
            
        }

        GLuint layout() const {return _layout; }
        GLuint numComponents() const {return _numComponents;}
        GLsizei stride() const {return _stride;}
        void* offset() const {return _offset;}
        GLenum type() const{return T;}


    private:
        GLuint _layout;
        GLuint _numComponents;
        GLsizei _stride;
        void* _offset;
    };
}