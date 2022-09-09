#include "gltimemeasure.h"

mygl::TimeMeasure::TimeMeasure()
{

}

mygl::TimeMeasure::~TimeMeasure() noexcept
{

}

std::map<std::string, std::chrono::time_point<std::chrono::steady_clock>> _timePoints;

mygl::TimeMeasure& mygl::TimeMeasure::getInstance()
{
    static mygl::TimeMeasure instance;
    return instance;
}


