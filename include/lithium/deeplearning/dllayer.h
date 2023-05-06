#pragma once

#include <functional>
#include "dlmatrix.h"

namespace lithium
{
    struct Layer
    {
        Matrix weights;
        std::pair<std::function<Matrix(const Matrix&)>, std::function<Matrix(const Matrix&)>> activationFunction;
        Matrix activations;
    };
}