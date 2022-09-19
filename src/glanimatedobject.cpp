#include "glanimatedobject.h"

lithium::AnimatedObject::AnimatedObject(std::shared_ptr<std::vector<lithium::Mesh*>> meshes, lithium::ImageTexture* texture, lithium::ImageTexture* specular)
    : lithium::Object{meshes->at(0), texture, specular}, _meshes{meshes}
{
    _currentFrame = _meshes->begin();
}

lithium::AnimatedObject::AnimatedObject(const lithium::AnimatedObject& other) : lithium::Object{other}, _iAnimatedObject{other._iAnimatedObject}, _looping{other._looping}, _playing{other._playing}, _interval{other._interval}, _duration{other._interval}
{
    _meshes = other._meshes;
    _currentFrame = _meshes->begin();
}

lithium::AnimatedObject::AnimatedObject(const lithium::Object& other) : lithium::Object{other}
{

}

lithium::AnimatedObject::~AnimatedObject() noexcept
{

}

void lithium::AnimatedObject::update(float dt)
{
    lithium::Object::update(dt);
    if(!_playing)
    {
        return;
    }
    _duration -= dt;
    if(_duration <= 0)
    {
        ++_currentFrame;
        if(_currentFrame == _meshes->end())
        {
            if(_looping)
            {
                _currentFrame = _meshes->begin();
            }
            else
            {
                _playing = false;
                if(_iAnimatedObject)
                {
                    _iAnimatedObject->onAnimationPlayedOnce(this);
                }
                return;
            }
        }
        setMesh(*_currentFrame);
        _duration = _interval;
    }
}