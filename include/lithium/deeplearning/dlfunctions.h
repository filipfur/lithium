#pragma once

#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include "dlmatrix.h"

#define TRANSFORM(v, expr) std::transform(v.begin(), v.end(), v.begin(), expr)

namespace lithium
{
    Matrix sigmoid(const Matrix& m)
    {
        Matrix result(m);
        for(auto it = result.begin(); it != result.end(); ++it)
        {
            Vector& vec = *it;
            TRANSFORM(vec, [](Real x) {return 1.0f / (1.0f + std::exp(-x));});
        }
        return result;
    }

    Matrix sigmoid_d(const Matrix& m)
    {
        Matrix result(m);
        for(auto it = result.begin(); it != result.end(); ++it)
        {
            Vector& vec = *it;
            TRANSFORM(vec, [](Real x) {return x * (1.0f - x);});
        }
        return result;
    }

    Matrix passthru(const Matrix& m)
    {
        return m;
    }

    void relu(std::vector<float>& v)
    {
        TRANSFORM(v, [](float x) {return std::max(0.0f, x);});
    }

    void softmax(std::vector<float>& v)
    {
        TRANSFORM(v, [](float x) {return std::exp(x);});
        float sum = std::accumulate(v.begin(), v.end(), 0.0f);
        TRANSFORM(v, [sum](float x) {return x / sum;});
    }

    /*
    def softmax(Z):
    e_x = np.exp(Z)
    A= e_x / np.sum(np.exp(Z))  
    cache=Z
    return A,cache  
    */

    float relu_d(const std::vector<float>& v0, std::vector<float>& v1)
    {
        std::transform(v0.begin(), v0.end(), v1.begin(), [](float x) {return x > 0.0f ? 1.0f : 0.0f;});
        return 0.0f;
    }

    /*float sigmoid_d(float x, float y)
    {
        float s = sigmoid(x);
        return y * s * (1.0f - s);
    }*/
}