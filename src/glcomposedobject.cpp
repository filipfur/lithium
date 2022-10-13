#include "glcomposedobject.h"

lithium::ComposedObject::ComposedObject(lithium::Mesh* mesh, lithium::ImageTexture* texture,
        std::vector<lithium::Object*> objects)
    : lithium::Object{mesh, texture}, _objects{objects}
{

}

lithium::ComposedObject::ComposedObject(const lithium::ComposedObject& other) : lithium::Object{other}
{
}

lithium::ComposedObject::ComposedObject(const lithium::Object& other) : lithium::Object{other}
{

}

lithium::ComposedObject::~ComposedObject() noexcept
{

}