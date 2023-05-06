#pragma once

#include <cmath>
#include <vector>
#include "dlreal.h"
#include <numeric>
#include <algorithm>

#define TRANSFORM(v, expr) std::transform(v.begin(), v.end(), v.begin(), expr)

namespace lithium
{
    float sigmoid(std::vector<float>& v)
    {
        TRANSFORM(v, [](float x) {return 1.0f / (1.0f + exp(-x));});
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
    }

    float sigmoid_d(float x, float y)
    {
        float s = sigmoid(x);
        return y * s * (1.0f - s);
    }
}