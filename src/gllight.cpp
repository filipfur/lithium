#include "gllight.h"

mygl::Light::Light(mygl::Mesh* mesh, mygl::ShaderProgram* shaderProgram)
    : mygl::Object{mesh, shaderProgram, nullptr, nullptr}
{
}

mygl::Light::Light(const mygl::Light& other) : mygl::Object{other}
{

}

mygl::Light::Light(const mygl::Object& other) : mygl::Object{other}
{

}

mygl::Light::~Light() noexcept
{

}