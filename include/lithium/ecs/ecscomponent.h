#pragma once

#include <any>
#include <algorithm>
#include <vector>
#include "ecsentity.h"

namespace ecs
{
    extern uint32_t _nextComponentId;

    template <class T, uint32_t U=0>
    class Component
    {
    public:
        Component()
        {

        }

        static void attach(Entity& entity)
        {
            if(_id == 0)
            {
                _id = _nextComponentId;
                _nextComponentId = _nextComponentId << 1;
            }
            entity.addComponent(_id);
            _ts.push_back(T{});
        }

        static void detach(Entity& entity)
        {
            //assert(_id != 0);
            entity.removeComponent(_id);
            //_ts.push_back(T{}); REMOVE?!!
        }

        static std::vector<T> _ts;
        static uint32_t _id;
    };

    template <typename T>
    void attach(Entity& entity)
    {
        T::attach(entity);
    }

    template <typename T, typename U>
    void attach(Entity& entity)
    {
        T::attach(entity);
        U::attach(entity);
    }

    template <typename T, typename U, typename V>
    void attach(Entity& entity)
    {
        T::attach(entity);
        U::attach(entity);
        V::attach(entity);
    }

    template <typename T, typename U, typename V, typename W>
    void attach(Entity& entity)
    {
        T::attach(entity);
        U::attach(entity);
        V::attach(entity);
        W::attach(entity);
    }

    template <typename T, typename U, typename V, typename W, typename X>
    void attach(Entity& entity)
    {
        T::attach(entity);
        U::attach(entity);
        V::attach(entity);
        W::attach(entity);
        X::attach(entity);
    }

    template <class T, uint32_t U>
    std::vector<T> Component<T,U>::_ts{};

    template <class T, uint32_t U>
    uint32_t Component<T,U>::_id{0};

}