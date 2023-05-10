#include <iostream>

#include "dlneuralnetwork.h"
#include "dlfunctions.h"

#define printVar(x) std::cout << #x << " = " << x << std::endl

using vec = lithium::Vector;
using mat = lithium::Matrix;

int main(int argc, const char* argv[])
{
    lithium::Layer inputLayer{
        mat{},
        std::make_pair(lithium::passthru, lithium::passthru),
        mat{
            vec{0.0f, 0.0f},
            vec{0.0f, 1.0f},
            vec{1.0f, 0.0f},
            vec{1.0f, 1.0f}
        }
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

    printVar(inputLayer.activations);
    printVar(hiddenLayer.weights);
    printVar(outputLayer.weights);

    lithium::NeuralNetwork net{inputLayer, hiddenLayer, outputLayer};

    for(int epoch{0}; epoch < 3000; ++epoch)
    {
        net.forward();
        mat error = truth - net.outputLayer()->activations;
        if(epoch % 250 == 0)
        {
            printVar(error.sum());
        }
        mat dZ = error * LR;
        net.backward(dZ);
    }

    auto test = [&](const mat& X_test)
    {
        printVar(X_test);
        printVar(net.test(X_test));
    };

    test(mat{vec{0.0f, 0.0f}});
    test(mat{vec{0.0f, 1.0f}});
    test(mat{vec{1.0f, 0.0f}});
    test(mat{vec{1.0f, 1.0f}});

    return 0;
}