#pragma once

#include <glad/glad.h>

#define LITHIUM_GLELEMENT_DEF(classRef, bindFunc, bindType) classRef* classRef::_bound{nullptr}; \
void classRef::bind() { \
    if(_bound != this) \
    { \
        bindFunc(bindType, _id); \
        _bound = this; \
    } \
} \
void classRef::unbind() { \
    _bound = nullptr; \
    bindFunc(bindType, 0); \
} \

namespace lithium
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