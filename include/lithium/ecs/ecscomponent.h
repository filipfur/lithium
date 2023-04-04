#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include "ecsentity.h"
#include "ecsconstants.h"

//#define ECS_TRACE TRUE

namespace ecs
{
    extern uint32_t _nextComponentNumber;

    template <class T, uint32_t Variant=0, bool Static=false>
    class Component
    {
    public:
        using value_type = T;

        Component()
        {

        }

        static void attach(Entity& entity)
        {
            /*if(_bitSignature == 0)
            {
                //_number = _nextComponentNumber++;
                //_bitSignature = (1 << _number);
                if(_number >= 32)
                {
                    std::cerr << "Error: Reached maximum amount of Components. (32). Consider chaning underlying type to uint64_t." << std::endl;
                    exit(1);
                }
            }*/
#ifdef ECS_TRACE
            std::cout << "attaching component#" << _number << " b" << bitSignature() << " " << typeid(T).name() << std::endl;
#endif
            entity.addComponent(bitSignature());
            //_ts.push_back(T{});
        }

        template <typename... Args>
        static void attach(Entity& entity, Args... args)
        {
            attach(entity);
            attach(args...);
        }

        static void detach(Entity& entity)
        {
            //assert(_bitSignature != 0);
            entity.removeComponent(bitSignature());
            //_ts.push_back(T{}); REMOVE?!!
        }

        static void increment(Entity& entity, bool dirty)
        {
            if(dirty)
            {
                ++_versions[entity.id()];
#ifdef ECS_TRACE
                std::cout << "component#" << _number << " version=" << (int)_versions[entity.id()] <<  " " << typeid(T).name() << std::endl;
#endif
            }
        }

        static T& get(size_t index=0)
        {
            return Static ? _ts[0] : _ts[index];
        }

        static void set(T& t, size_t index=0)
        {
            if(Static)
            {
                _ts[0] = t;
                refreshAll();
            }
            else
            {
                _ts[index] = t;
                ++_versions[index];
            }
        }

        static uint8_t version(size_t index=0)
        {
            return _versions[index];
        }

        static bool compare(Entity& entity, const uint8_t& version)
        {
            return _versions[entity.id()] == version;
        }

        static void refreshAll()
        {
            for(auto i{0}; i < ecs::GLOBAL_MAX_ENTITIES; ++i)
            {
                refresh(i);
            }
        }

        static void refresh(size_t index=0)
        {
            ++_versions[index];
            //std::cout << "component#" << _number << " version=" << (int)_versions[index] << std::endl;
        }

        static uint32_t bitSignature()
        {
            if(_bitSignature == 0)
            {
                _bitSignature = (1 << _number);
            }
            return _bitSignature;
        }

        static uint32_t _number;
        static T _ts[];
        static uint8_t _versions[]; // Version of this component for a specific entity.

    private:
        static uint32_t _bitSignature;
    };

    template <typename... T>
    void attach(Entity& entity)
    {
        (T::attach(entity), ...); // fold expression
    }


    /*template <class T, uint32_t U>
    std::vector<T> Component<T,U>::_ts{};*/
    template <class T, uint32_t Variant, bool Static>
    T Component<T,Variant,Static>::_ts[Static ? 1 : ecs::GLOBAL_MAX_ENTITIES] = {};

    template <class T, uint32_t Variant, bool Static>
    uint8_t Component<T,Variant,Static>::_versions[ecs::GLOBAL_MAX_ENTITIES] = {};

    template <class T, uint32_t Variant, bool Static>
    uint32_t Component<T,Variant,Static>::_number{_nextComponentNumber++};

    template <class T, uint32_t Variant, bool Static>
    uint32_t Component<T,Variant,Static>::_bitSignature{0};//(uint32_t)((uint32_t)0x00000001 << (uint32_t)_number)}; This doesn't work on Windows.
}