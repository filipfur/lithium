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

    Matrix passthru_d(const Matrix& m)
    {
        return Matrix(m.rows(), m.columns(), 1.0f);
    }
    
    Matrix relu(const Matrix& m)
    {
        Matrix result(m);
        for(auto it = result.begin(); it != result.end(); ++it)
        {
            Vector& vec = *it;
            TRANSFORM(vec, [](Real x) {return std::max(0.0f, x);});
        }
        return result;
    }

    Matrix relu_d(const Matrix& m)
    {
        Matrix result(m);
        for(auto it = result.begin(); it != result.end(); ++it)
        {
            Vector& vec = *it;
            TRANSFORM(vec, [](Real x) {return x > 0.0f ? 1.0f : 0.0f;});
        }
        return result;
    }

    Matrix softmax(const Matrix& m)
    {
        Matrix result(m);
        for(auto it = result.begin(); it != result.end(); ++it)
        {
            Vector& vec = *it;
            Real sum = std::accumulate(vec.begin(), vec.end(), 0.0f);
            TRANSFORM(vec, [sum](Real x) {return std::exp(x) / sum;});
        }
        return result;
    }

    Matrix softmax_d(const Matrix& m)
    {
        Matrix result(m);
        for(auto it = result.begin(); it != result.end(); ++it)
        {
            Vector& vec = *it;
            TRANSFORM(vec, [](Real x) {return x * (1.0f - x);});
        }
        return result;
    }
}