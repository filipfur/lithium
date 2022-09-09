#include "glanimatedobject.h"

mygl::AnimatedObject::AnimatedObject(std::shared_ptr<std::vector<mygl::Mesh*>> meshes, mygl::ShaderProgram* shaderProgram, mygl::Texture* texture, mygl::Texture* specular)
    : mygl::Object{meshes->at(0), shaderProgram, texture, specular}, _meshes{meshes}
{
    _currentFrame = _meshes->begin();
}

mygl::AnimatedObject::AnimatedObject(const mygl::AnimatedObject& other) : mygl::Object{other}, _iAnimatedObject{other._iAnimatedObject}, _looping{other._looping}, _playing{other._playing}, _interval{other._interval}, _duration{other._interval}
{
    _meshes = other._meshes;
    _currentFrame = _meshes->begin();
}

mygl::AnimatedObject::AnimatedObject(const mygl::Object& other) : mygl::Object{other}
{

}

mygl::AnimatedObject::~AnimatedObject() noexcept
{

}

void mygl::AnimatedObject::update(float dt)
{
    mygl::Object::update(dt);
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