
#pragma once

#include "glupdateable.h"
#include "glperiodictask.h"
#include "glturntask.h"

namespace lithium
{
    class TaskScheduler : public lithium::Updateable
    {
    public:
        TaskScheduler();
        virtual ~TaskScheduler() noexcept;

        static TaskScheduler& getInstance();

        static void append(lithium::Updateable* updateable);

        virtual void update(float dt) override;

        void turnStarted();

        static PeriodicTask* startPeriodic(float time, const std::function<void()>& callback)
        {
            PeriodicTask* ptr = new PeriodicTask(time, callback, true);
            TaskScheduler::append(ptr);
            return ptr;
        }

        static PeriodicTask* startDelayed(float time, const std::function<void()>& callback, float *progress=nullptr)
        {
            PeriodicTask* ptr = new PeriodicTask(time, callback, false, progress);
            TaskScheduler::append(ptr);
            return ptr;
        }

        static TurnTask* startTurnTask(int delayTurns, const std::function<void()>& callback, bool periodic=false)
        {
            TurnTask* ptr = new TurnTask(delayTurns, callback, periodic);
            getInstance()._turnTasks.push_back(ptr);
            return ptr;
        }

        TaskScheduler(TaskScheduler const&) = delete;
        void operator=(TaskScheduler const&)  = delete;

    private:
        std::vector<lithium::Updateable*> _iUpdateables;
        std::vector<lithium::Updateable*> _iUBuffer;
        std::vector<TurnTask*> _turnTasks;
    };
}