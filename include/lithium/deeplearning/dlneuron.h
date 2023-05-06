#pragma once

#include <functional>
#include "dlreal.h"

const static auto mulBy2 = [](Real val){ return val + val; };

namespace lithium
{
    class Neuron
    {
    public:
        Neuron() : Neuron{mulBy2}
        {

        }
    
        Neuron(const std::function<Real(Real)>& operation) : _operation{operation}
        {
            
        }

        virtual ~Neuron() noexcept
        {

        }

        void forward(Real value)
        {
            _value = _operation(value);
            if(_next)
            {
                _next->forward(_value);
            }
        }

        void connect(Neuron* other)
        {
            setNext(other);
            other->setPrev(this);
        }

        float value() const
        {
            return _value;
        }

        float gradient() const
        {
            return _gradient;
        }

    private:
        void setNext(Neuron* next)
        {
            _next = next;
        }

        void setPrev(Neuron* prev)
        {
            _prev = prev;
        }

        Real _gradient{0.0f};
        Real _value{0.0f};
        std::function<Real(Real)> _operation;
        Neuron* _next{nullptr};
        Neuron* _prev{nullptr};
    };
}