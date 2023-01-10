#pragma once

#include "glianimation2.h"

namespace lithium
{
    class Animation2;

    class IAnimation2
    {
    public:
        virtual void onAnimationPlayedOnce(lithium::Animation2* animation) = 0;
    };
}