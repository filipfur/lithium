#pragma once

#include <vector>
#include <cassert>
#include "dllayer.h"

namespace lithium
{
    class NeuralNetwork
    {
    public:
        NeuralNetwork(const std::initializer_list<Layer>& layers)
            : _layers{layers}, _inputLayer{_layers.begin()}, _outputLayer{_layers.end() - 1}
        {
        }

        void forward()
        {
            for(auto it = _inputLayer + 1; it != _layers.end(); ++it)
            {
                it->activations = it->activationFunction.first((it - 1)->activations.dot(it->weights));
            }
        }

        void backward(const Matrix& dZ)
        {
            lithium::Matrix d = dZ;
            for(auto it = _outputLayer; it != _inputLayer; --it)
            {
                it->weights += (it - 1)->activations.transpose().dot(d);
                if((it - 1) == _inputLayer)
                {
                    break;
                }
                d = d.dot(it->weights.transpose()) * (it - 1)->activationFunction.second((it - 1)->activations);
            }
        }

        Matrix test(const Matrix& X_test)
        {
            _inputLayer->activations = X_test;
            forward();
            return _outputLayer->activations;
        }

        void setLearningRate(float learningRate)
        {
            _learningRate = learningRate;
        }

        float learningRate() const
        {
            return _learningRate;
        }

        std::vector<Layer>::iterator inputLayer()
        {
            return _inputLayer;
        }

        std::vector<Layer>::iterator outputLayer()
        {
            return _outputLayer;
        }

    private:
        std::vector<Layer> _layers;
        std::vector<Layer>::iterator _inputLayer;
        std::vector<Layer>::iterator _outputLayer;
        float _learningRate = 0.1f;
    };
}