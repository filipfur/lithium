
#pragma once

#include <functional>
#include "task.h"

class TurnTask : public Task
{
public:
    TurnTask(int delayTurns, const std::function<void()>& callback, bool periodic=true)
    : _delayTurns{delayTurns}, _callback{callback}, _periodic{periodic} {}
    
    virtual ~TurnTask() noexcept
    {

    }

    void turnStarted()
    {
        if(_delayTurns == 0)
        {
            if(_periodic)
            {
                _delayTurns = _delayTurnsMax;
            }
            _callback();
        }
        --_delayTurns;
    }

    virtual float remaining() const override
    {
        return (float)_delayTurns / (float)_delayTurnsMax;
    }

    virtual float progress() const override
    {
        return 1.0f - remaining();
    }

    virtual void cancel() override
    {
        Task::cancel();
        _delayTurns = -1;
    }

    virtual bool isExpired() const override
    {
        return !_periodic && _delayTurns < 0;
    }

    virtual void restart() override
    {
        _delayTurns = _delayTurnsMax;
    }

private:
    int _delayTurnsMax;
    int _delayTurns;
    bool _periodic;
    std::function<void()> _callback;
};
