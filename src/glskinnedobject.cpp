#include "glskinnedobject.h"

lithium::SkinnedObject::SkinnedObject(lithium::Mesh* mesh, lithium::ImageTexture* texture, lithium::ImageTexture* specular)
    : lithium::Object{mesh, texture, specular}
{
}

lithium::SkinnedObject::SkinnedObject(const lithium::SkinnedObject& other) : lithium::Object{other}, lithium::Animation2{other}
{
}

lithium::SkinnedObject::~SkinnedObject() noexcept
{

}

void lithium::SkinnedObject::update(float dt)
{
    lithium::Object::update(dt);
}