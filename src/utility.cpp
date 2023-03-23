#include "utility.h"

#include <sstream>
#include <iomanip>

glm::vec3 utility::directionVector(float radians)
{
    return glm::vec3{glm::sin(radians), 0.0f, glm::cos(radians)};
}

float utility::lerp(float a, float b, float f)
{
    return a * (1.0 - f) + (b * f);
}

float utility::maxf(float a, float b)
{
    return a > b ? a : b;
}

float utility::minf(float a, float b)
{
    return a < b ? a : b;
}

float utility::clamp(float f, float a, float b)
{
    return utility::minf(b, utility::maxf(a, f));
}

void utility::printMatrix(const glm::mat4& matrix)
{
    std::cout << "mat4 [ ";
    printGLM(matrix, 16);
    std::cout << " ]" << std::endl;
}

void utility::printVector(const glm::vec2& vec)
{
    std::cout << "vec2 [ ";
    printGLM(vec, 2);
    std::cout << " ]" << std::endl;
}

void utility::printVector(const glm::vec3& vec)
{
    std::cout << "vec3 [ ";
    printGLM(vec, 3);
    std::cout << " ]" << std::endl;
}

std::string utility::vectorToString(const glm::vec3& vec, const int precision)
{
    return GLMtoString(vec, 3, precision);
}

std::string utility::hexStr(char* data, int len)
{
    std::stringstream ss;
    ss << std::hex;

    std::string delim{""};

    for(int i(0); i < len; ++i )
    {
        if(i != 0 && i % 8 == 0)
        {
            ss << std::endl;
        }
        ss << delim << std::setw(2) << std::setfill('0') << (int)(data[i] & 0xFF);
        delim = " ";
    }

    return ss.str();
}

float utility::angle(const glm::vec3& p0, const glm::vec3& p1)
{
    const glm::vec3 d{p1 - p0};
    return atan2f(d.x, d.z);
}

glm::vec3 utility::lerp(const glm::vec3& a, const glm::vec3& b, float t)
{
    return a * (1.f - t) + b * t;
}

//static const float utility::pi{3.14159265359f};
//static const float utility::pi2{pi * 2.0f};
//static const float utility::pih{pi * 0.5f};