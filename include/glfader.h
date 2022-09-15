#pragma once

#include "gltaskscheduler.h"
#include <functional>

namespace lithium
{
    class Fader
    {
    public:
        Fader(float fadeTime, float fadeFrom, float fadeTo, const std::function<void()>& callback)
        : _fadeTime{fadeTime}, _fadeFrom{fadeFrom}, _fadeTo{fadeTo}
        {
            _fadeTimer = TaskScheduler::startDelayed(fadeTime, [this, callback]{
                callback();
                _fadeTimer = nullptr;
            });
        }

        virtual ~Fader() noexcept
        {
            if(_fadeTimer)
            {
                _fadeTimer->cancel();
                _fadeTimer = nullptr;
            }
        }

        float value() const
        {
            return _fadeFrom + _fadeTimer->progress() * (_fadeTo - _fadeFrom);
        }

        float fadeTime() const { return _fadeTime; }

        float fadeFrom() const { return _fadeFrom; }

        float fadeTo() const { return _fadeTo; }

    private:
        float _fadeTime;
        float _fadeFrom;
        float _fadeTo;
        PeriodicTask* _fadeTimer{nullptr};
    };
}