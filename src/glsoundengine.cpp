#include "glsoundengine.h"

mygl::SoundEngine::SoundEngine()
{
    
}

mygl::SoundEngine::~SoundEngine() noexcept
{
}

mygl::SoundEngine& mygl::SoundEngine::getInstance()
{
    static mygl::SoundEngine instance;
    return instance;
}