#include "glperiodictask.h"

lithium::PeriodicTask::PeriodicTask(float periodTime, const std::function<void()>& callback, bool periodic, float* progressPtr) : _periodTime{periodTime}, _progressPtr{progressPtr}, _periodTimeMax{periodTime}, _callback{callback}, _periodic{periodic}
{

}

lithium::PeriodicTask::~PeriodicTask() noexcept
{

}

void lithium::PeriodicTask::update(float dt)
{
    if(isCanceled())
    {
        return;
    }
    _periodTime -= dt;
    if(_progressPtr)
    {
        *_progressPtr = progress();
    }
    if(_periodTime <= 0)
    {
        _callback();
        if(_periodic)
        {
            _periodTime = _periodTimeMax;
        }
    }
}