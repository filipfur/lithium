#pragma once

#include "globject.h"

namespace lithium
{
    class ComposedObject : public Object
    {
    public:
        ComposedObject(lithium::Mesh* mesh, lithium::ImageTexture* texture, const std::vector<lithium::Object*>& objects);
        ComposedObject(const ComposedObject& other);
        ComposedObject(const Object& other);
        virtual ~ComposedObject() noexcept;

    private:
        std::vector<lithium::Object*> _objects;
    };
}