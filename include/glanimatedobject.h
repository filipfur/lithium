#pragma once

#include "globject.h"
#include "ianimatedobject.h"

namespace mygl
{
    class AnimatedObject : public mygl::Object
    {
    public:
        AnimatedObject(std::shared_ptr<std::vector<mygl::Mesh*>> meshes, mygl::ShaderProgram* shaderProgram, mygl::Texture* texture, mygl::Texture* specular=nullptr);
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

        void setIAnimatedObject(mygl::IAnimatedObject* iAnimatedObject)
        {
            mygl::Object::setIObject(iAnimatedObject);
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
        std::shared_ptr<std::vector<mygl::Mesh*>> _meshes;
        std::vector<mygl::Mesh*>::iterator _currentFrame;
        float _interval{1.0f / 24.0f};
        float _duration{_interval};
        mygl::IAnimatedObject* _iAnimatedObject{nullptr};
        bool _looping{false};
        bool _playing{false};
    };
}