#pragma once

#include <chrono>
#include <string>
#include <map>
#include <cassert>
#include <iostream>
#include <iomanip>

namespace lithium
{
    class TimeMeasure
    {
    public:

        class Handle
        {
            public:
                Handle(const std::string& label) : _label{label}
                {

                }

                ~Handle()
                {
                    end();
                }

                template <typename T=std::chrono::microseconds>
                void end()
                {
                    TimeMeasure::end<T>(_label);
                }

                const std::string label() const
                {
                    return _label;
                }

            private:
                const std::string _label;

        };

        TimeMeasure();
        virtual ~TimeMeasure() noexcept;
        static TimeMeasure& getInstance();

        static const std::string& start(const std::string& label, bool enabled=true)
        {
            if(enabled)
            {
                auto now = std::chrono::steady_clock::now();
                TimeMeasure& inst = getInstance();
                assert(inst._timePoints.find(label) == inst._timePoints.end());
                inst._timePoints.emplace(label, now);
            }
            return label;
        }

        template <typename T=std::chrono::microseconds>
        static void end(const std::string& label)
        {
            TimeMeasure& inst = getInstance();
            auto now = std::chrono::steady_clock::now();
            
            auto it = inst._timePoints.find(label);
            if(it != inst._timePoints.end())
            {
                std::cout << "Measured '" << label << "': " << std::fixed << std::setprecision(1) << std::chrono::duration_cast<T>(now - it->second).count() / 1000.0f << "[ms]" << std::endl;
                inst._timePoints.erase(it);
            }
        }

        TimeMeasure(TimeMeasure const&) = delete;
        void operator=(TimeMeasure const&)  = delete;

    private:
        std::map<std::string, std::chrono::time_point<std::chrono::steady_clock>> _timePoints;
    };
}
