#pragma once

#include "glianimation.h"

namespace lithium
{
    class Animation;

    class IAnimation
    {
    public:
        virtual void onAnimationPlayedOnce(lithium::Animation* animation) = 0;
    };
}