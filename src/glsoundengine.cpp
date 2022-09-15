#include "glsoundengine.h"

lithium::SoundEngine::SoundEngine()
{
    
}

lithium::SoundEngine::~SoundEngine() noexcept
{
}

lithium::SoundEngine& lithium::SoundEngine::getInstance()
{
    static lithium::SoundEngine instance;
    return instance;
}