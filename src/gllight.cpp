#include "gllight.h"

lithium::Light::Light(lithium::Mesh* mesh)
    : lithium::Object{mesh, nullptr, nullptr}
{
}

lithium::Light::Light(const lithium::Light& other) : lithium::Object{other}
{

}

lithium::Light::Light(const lithium::Object& other) : lithium::Object{other}
{

}

lithium::Light::~Light() noexcept
{

}