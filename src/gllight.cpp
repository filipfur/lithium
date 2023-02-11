#include "gllight.h"

lithium::Light::Light(std::shared_ptr<lithium::Mesh> mesh)
    : lithium::Object{mesh, {}}
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