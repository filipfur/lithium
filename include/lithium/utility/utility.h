#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>
#include <sstream>

namespace utility
{
    glm::vec3 directionVector(float radians);

    float lerp(float a, float b, float f);

    glm::vec3 lerp(const glm::vec3 &a, const glm::vec3 &b, float t);

    float maxf(float a, float b);

    float minf(float a, float b);

    float clamp(float f, float a, float b);

    void printMatrix(const glm::mat4 &matrix);

    template <typename T>
    void printGLM(const T &vec, int n)
    {
        std::string delim{""};
        const float *pSource = (const float *)glm::value_ptr(vec);
        for (int i{0}; i < n; ++i)
        {
            std::cout << delim << pSource[i];
            delim = ", ";
        }
    }

    template <typename T>
    std::string GLMtoString(const T &vec, int n, int precision=6)
    {
        std::string delim{""};
        std::string rval{""};
        const float *pSource = (const float *)glm::value_ptr(vec);
        std::ostringstream os;
        os.precision(precision);
        for (int i{0}; i < n; ++i)
        {
            os << std::fixed << delim << pSource[i];
            delim = ", ";
        }
        return os.str();
    }

    std::string vectorToString(const glm::vec3& vec, int precision=2);

    void printVector(const glm::vec2 &vec);
    void printVector(const glm::vec3 &vec);

    std::string hexStr(char *data, int len);

    float angle(const glm::vec3 &p0, const glm::vec3 &p1);

    float cross2d(const glm::vec3& a, const glm::vec3& b);

    static const float pi{3.14159265359f};
    static const float pi2{pi * 2.0f};
    static const float pih{pi * 0.5f};
}