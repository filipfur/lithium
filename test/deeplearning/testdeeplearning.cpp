#include <iostream>

#include "dlneuralnetwork.h"
#include "dlfunctions.h"

#define printVar(x) std::cout << #x << " = " << x << std::endl

using vec = lithium::Vector;
using mat = lithium::Matrix;

int main(int argc, const char* argv[])
{
    lithium::Layer inputLayer{
        mat{
            vec{0.0f, 0.0f},
            vec{0.0f, 1.0f},
            vec{1.0f, 0.0f},
            vec{1.0f, 1.0f}
        },
        std::make_pair(lithium::passthru, lithium::passthru)
    };

    lithium::Layer hiddenLayer{
        mat::uniform(2, 3, 0.01f, 0.99f),
        std::make_pair(lithium::sigmoid, lithium::sigmoid_d)
    };

    lithium::Layer outputLayer{
        mat::uniform(3, 1, 0.01f, 0.99f),
        std::make_pair(lithium::passthru, lithium::passthru)
    };

    mat truth{
        vec{0.0f},
        vec{1.0f},
        vec{1.0f},
        vec{0.0f}
    };

    float LR = 0.1f;

    printVar(inputLayer.weights);
    printVar(hiddenLayer.weights);
    printVar(outputLayer.weights);

    for(int epoch{0}; epoch < 3000; ++epoch)
    {
        hiddenLayer.activations = hiddenLayer.activationFunction.first(inputLayer.weights.dot(hiddenLayer.weights));
        outputLayer.activations = outputLayer.activationFunction.first(hiddenLayer.activations.dot(outputLayer.weights));
        mat error = truth - outputLayer.activations;
        
        if(epoch % 250 == 0)
        {
            printVar(error.sum());
        }

        mat dZ = error * LR;
        outputLayer.weights += hiddenLayer.activations.transpose().dot(dZ);
        mat dH = dZ.dot(outputLayer.weights.transpose()) * hiddenLayer.activationFunction.second(hiddenLayer.activations);
        hiddenLayer.weights += inputLayer.weights.transpose().dot(dH);
    }

    auto test = [&](const mat& X_test)
    {
        mat act_hidden = lithium::sigmoid(X_test.dot(hiddenLayer.weights));
        mat res = act_hidden.dot(outputLayer.weights);
        printVar(X_test);
        printVar(res);
    };

    test(mat{vec{0.0f, 0.0f}});
    test(mat{vec{0.0f, 1.0f}});
    test(mat{vec{1.0f, 0.0f}});
    test(mat{vec{1.0f, 1.0f}});

    return 0;
}