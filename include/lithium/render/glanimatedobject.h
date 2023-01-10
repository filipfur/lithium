#pragma once

#include "globject.h"
#include "glanimation2.h"

namespace lithium
{
    class AnimatedObject : public lithium::Object, public lithium::Animation2
    {
    public:
        AnimatedObject(std::shared_ptr<std::vector<lithium::Mesh*>> meshes, lithium::ImageTexture* texture, lithium::ImageTexture* specular=nullptr);
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
        std::shared_ptr<std::vector<lithium::Mesh*>> _meshes;
    };
}