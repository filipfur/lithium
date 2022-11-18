#include "gltaskscheduler.h"

lithium::TaskScheduler::TaskScheduler()
{

}

lithium::TaskScheduler::~TaskScheduler() noexcept
{

}

std::vector<lithium::Updateable*> _iUpdateables;
std::vector<lithium::Updateable*> _iUBuffer;
std::vector<lithium::TurnTask*> _turnTasks;

void lithium::TaskScheduler::update(float dt)
{
    if(!_iUBuffer.empty())
    {
        _iUpdateables.insert(_iUpdateables.end(), _iUBuffer.begin(), _iUBuffer.end());
        _iUBuffer.clear();
    }
    auto it = _iUpdateables.begin();
    while(it != _iUpdateables.end())
    {
        lithium::Updateable* iUpdateable = (*it);
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
    /*std::remove_if(_iUpdateables.begin(), _iUpdateables.end(), [](lithium::Updateable* iUpdateable){
        if(auto periodicTask = dynamic_cast<PeriodicTask*>(iUpdateable))
        {
            return (periodicTask->isExpired() || periodicTask->isCanceled());
        }
    });*/
}

void lithium::TaskScheduler::turnStarted()
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

lithium::TaskScheduler& lithium::TaskScheduler::getInstance()
{
    static lithium::TaskScheduler instance;
    return instance;
}

void lithium::TaskScheduler::append(lithium::Updateable* iUpdateable)
{
    lithium::TaskScheduler* inst =  &lithium::TaskScheduler::getInstance();
    inst->_iUBuffer.push_back(iUpdateable);
    int number = 1;
}