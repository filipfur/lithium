#pragma once

#include "globject.h"
#include "glanimation.h"

namespace lithium
{
    class AnimatedObject : public lithium::Object, public lithium::Animation
    {
    public:
        AnimatedObject(std::shared_ptr<std::vector<std::shared_ptr<lithium::Mesh>>> meshes, const std::vector<lithium::Object::TexturePointer>& textures);
        AnimatedObject(const AnimatedObject& other);
        AnimatedObject(const Object& other);
        virtual ~AnimatedObject() noexcept;

        virtual void update(float dt) override;

        virtual void onAnimationFrameChanged(int frame) override
        {
            setMesh(_meshes->at(frame));
        }

        virtual lithium::AnimatedObject* clone() const override
		{
			return new lithium::AnimatedObject(*this);
        }

    private:
        std::shared_ptr<std::vector<std::shared_ptr<lithium::Mesh>>> _meshes;
    };
}