#include "gllight.h"

lithium::Light::Light(std::shared_ptr<lithium::Mesh> mesh, const std::vector<Object::TexturePointer>& textures)
    : lithium::Object{mesh, textures}
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