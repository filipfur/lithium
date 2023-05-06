#include <iostream>

#include "lithium/deeplearning/dlneuralnetwork.h"

int main(int argc, const char* argv[])
{
    std::cout << "hge" << std::endl;

    lithium::NeuralNetwork net{
        {
            {new lithium::Neuron(), new lithium::Neuron()},
            {new lithium::Neuron(), new lithium::Neuron()},
            {new lithium::Neuron(), new lithium::Neuron()},
            {new lithium::Neuron(), new lithium::Neuron()},
            {new lithium::Neuron()}
        }
    };

    net.forward({0.1f, 0.1f});
    Real result = net.output();
    std::cout << "output: " << result << std::endl;

    return 0;
}