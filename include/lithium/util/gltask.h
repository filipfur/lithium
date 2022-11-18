
#pragma once

namespace lithium
{
    class Task
    {
    public:
        Task()
        {

        }
        
        virtual ~Task() noexcept
        {

        }

        virtual bool isExpired() const = 0;

        virtual void cancel()
        {
            _canceled = true;
        }

        virtual float progress() const = 0;
        virtual float remaining() const = 0;

        virtual void restart() = 0;

        bool isCanceled()
        {
            return _canceled;
        }

    private:
        bool _canceled{false};
    };
}