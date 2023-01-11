#include "glanimatedobject.h"

lithium::AnimatedObject::AnimatedObject(std::shared_ptr<std::vector<lithium::Mesh*>> meshes, lithium::ImageTexture* texture, lithium::ImageTexture* specular)
    : lithium::Object{meshes->at(0), texture, specular}, _meshes{meshes}, lithium::Animation2{"", static_cast<int>(meshes->size())}
{
}

lithium::AnimatedObject::AnimatedObject(const lithium::AnimatedObject& other) : lithium::Object{other}, lithium::Animation2{other}
{
    _meshes = other._meshes;
}

lithium::AnimatedObject::~AnimatedObject() noexcept
{

}

void lithium::AnimatedObject::update(float dt)
{
    lithium::Animation2::step(dt);
    lithium::Object::update(dt);
}