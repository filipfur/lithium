#pragma once

#include "iobject.h"

namespace lithium
{
    class AnimatedObject;

    class IAnimatedObject : public IObject
    {
    public:
        virtual void onAnimationPlayedOnce(lithium::AnimatedObject* animatedObject) = 0;
    };
}