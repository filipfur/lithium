#pragma once

#include "iobject.h"

namespace mygl
{
    class AnimatedObject;

    class IAnimatedObject : public IObject
    {
    public:
        virtual void onAnimationPlayedOnce(mygl::AnimatedObject* animatedObject) = 0;
    };
}