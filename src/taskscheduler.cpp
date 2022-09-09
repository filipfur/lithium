#include "taskscheduler.h"

TaskScheduler::TaskScheduler()
{

}

TaskScheduler::~TaskScheduler() noexcept
{

}

std::vector<IUpdateable*> _iUpdateables;
std::vector<IUpdateable*> _iUBuffer;
std::vector<TurnTask*> _turnTasks;

void TaskScheduler::update(float dt)
{
    if(!_iUBuffer.empty())
    {
        _iUpdateables.insert(_iUpdateables.end(), _iUBuffer.begin(), _iUBuffer.end());
        _iUBuffer.clear();
    }
    auto it = _iUpdateables.begin();
    while(it != _iUpdateables.end())
    {
        IUpdateable* iUpdateable = (*it);
        iUpdateable->update(dt);
        auto periodicTask = dynamic_cast<PeriodicTask*>(iUpdateable);
        if(periodicTask && (periodicTask->isExpired() || periodicTask->isCanceled()))
        {
            delete periodicTask;
            it = _iUpdateables.erase(it);
        }
        else
        {
            ++it;
        }
    }
    /*std::remove_if(_iUpdateables.begin(), _iUpdateables.end(), [](IUpdateable* iUpdateable){
        if(auto periodicTask = dynamic_cast<PeriodicTask*>(iUpdateable))
        {
            return (periodicTask->isExpired() || periodicTask->isCanceled());
        }
    });*/
}

void TaskScheduler::turnStarted()
{
    auto it = _turnTasks.begin();
    while(it != _turnTasks.end())
    {
        auto turnTask = *it;
        turnTask->turnStarted();
        if(turnTask->isExpired())
        {
            it = _turnTasks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

TaskScheduler& TaskScheduler::getInstance()
{
    static TaskScheduler instance;
    return instance;
}

void TaskScheduler::append(IUpdateable* iUpdateable)
{
    TaskScheduler* inst =  &TaskScheduler::getInstance();
    inst->_iUBuffer.push_back(iUpdateable);
    int number = 1;
}