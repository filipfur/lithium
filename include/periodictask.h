
#pragma once

#include "iupdateable.h"
#include "task.h"
#include <vector>
#include <functional>

class PeriodicTask : public Task, public IUpdateable
{
public:
    PeriodicTask(float periodTime, const std::function<void()>& callback, bool _periodic=true, float* progress=nullptr);
    virtual ~PeriodicTask() noexcept;

    virtual void update(float dt) override;

    virtual float remaining() const override
    {
        return _periodTime / _periodTimeMax;
    }

    virtual float progress() const override
    {
        return 1.0f - remaining();
    }

    virtual bool isExpired() const override
    {
        return !_periodic && remaining() <= 0;
    }

    virtual void cancel() override
    {
        Task::cancel();
    }

    virtual void restart() override
    {
        _periodTime = _periodTimeMax;
    }

private:
    float _periodTime;
    float _periodTimeMax;
    std::function<void()> _callback;
    bool _periodic;
    float* _progressPtr;
};