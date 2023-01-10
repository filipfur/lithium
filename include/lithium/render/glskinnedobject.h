#pragma once

#include "globject.h"

namespace lithium
{
    class SkinnedObject : public lithium::Object
    {
    public:
        SkinnedObject(lithium::Mesh* mesh, lithium::ImageTexture* texture, lithium::ImageTexture* specular=nullptr);
        SkinnedObject(const SkinnedObject& other);
        virtual ~SkinnedObject() noexcept;

        virtual void update(float dt) override;

    private:

    };
}