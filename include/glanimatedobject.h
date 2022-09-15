#pragma once

#include "globject.h"
#include "ianimatedobject.h"

namespace lithium
{
    class AnimatedObject : public lithium::Object
    {
    public:
        AnimatedObject(std::shared_ptr<std::vector<lithium::Mesh*>> meshes, lithium::Texture* texture, lithium::Texture* specular=nullptr);
        AnimatedObject(const AnimatedObject& other);
        AnimatedObject(const Object& other);
        virtual ~AnimatedObject() noexcept;

        virtual void update(float dt) override;

        void play()
        {
            _playing = true;
            _duration = _interval;
            _currentFrame = _meshes->begin();
            setMesh(_meshes->at(0));
        }

        void setIAnimatedObject(lithium::IAnimatedObject* iAnimatedObject)
        {
            lithium::Object::setIObject(iAnimatedObject);
            _iAnimatedObject = iAnimatedObject;
        }

        void setLooping(bool looping)
        {
            _looping = looping;
        }

        void setInterval(float interval)
        {
            _interval = interval;
        }

    private:
        std::shared_ptr<std::vector<lithium::Mesh*>> _meshes;
        std::vector<lithium::Mesh*>::iterator _currentFrame;
        float _interval{1.0f / 24.0f};
        float _duration{_interval};
        lithium::IAnimatedObject* _iAnimatedObject{nullptr};
        bool _looping{false};
        bool _playing{false};
    };
}