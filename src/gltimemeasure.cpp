#include "gltimemeasure.h"

lithium::TimeMeasure::TimeMeasure()
{

}

lithium::TimeMeasure::~TimeMeasure() noexcept
{

}

std::map<std::string, std::chrono::time_point<std::chrono::steady_clock>> _timePoints;

lithium::TimeMeasure& lithium::TimeMeasure::getInstance()
{
    static lithium::TimeMeasure instance;
    return instance;
}


