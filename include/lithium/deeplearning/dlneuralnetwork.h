#pragma once

#include <vector>
#include "dlneuron.h"

namespace lithium
{
    class NeuralNetwork
    {
    public:
        using Layer = std::vector<Neuron*>;

        NeuralNetwork(const std::vector<Layer>& layers) : _layers{layers}
        {
            for(auto it = _layers.begin() + 1; it != _layers.end(); ++it)
            {
                auto prev = it - 1;
                for(int i{0}; i < prev->size(); ++i)
                {
                    prev->at(i)->connect(i >= it->size() ? it->back() : it->at(i));
                }
            }
            _inputLayer = _layers.begin();
            _outputLayer = _layers.end() - 1;
        }

        void forward(const std::initializer_list<Real>& vals)
        {
            assert(vals.size() == _inputLayer->size());
            for(int i{0}; i < vals.size(); ++i)
            {
                _inputLayer->at(i)->forward(vals.begin()[i]);
            }
        }

        Real output(size_t index=0)
        {
            return _outputLayer->at(index)->value();
        }

        std::vector<Layer> _layers;
        std::vector<Layer>::iterator _inputLayer;
        std::vector<Layer>::iterator _outputLayer;
    };
}