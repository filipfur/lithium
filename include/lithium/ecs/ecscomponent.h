#pragma once

#include <algorithm>
#include <vector>
#include "ecsentity.h"
#include "ecsconstants.h"

namespace ecs
{
    extern uint32_t _nextComponentNumber;

    template <class T, uint32_t Variant=0, bool Static=false>
    class Component
    {
    public:
        using value_type = T;
        inline static constexpr int MAX_ENTITIES{Static ? 1 : ecs::GLOBAL_MAX_ENTITIES};

        Component()
        {

        }

        static void attach(Entity& entity)
        {
            if(_bitSignature == 0)
            {
                //_number = _nextComponentNumber++;
                //_bitSignature = (1 << _number);
                if(_number >= 32)
                {
                    std::cerr << "Error: Reached maximum amount of Components. (32). Consider chaning underlying type to uint64_t." << std::endl;
                    exit(1);
                }
            }
            std::cout << "attaching: " << _number << ", " << _bitSignature << std::endl;
            entity.addComponent(_bitSignature);
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
            entity.removeComponent(_bitSignature);
            //_ts.push_back(T{}); REMOVE?!!
        }

        static void increment(Entity& entity, bool dirty)
        {
            if(dirty)
            {
                ++_versions[entity.id()];
                std::cout << "component#" << _number << " version=" << (int)_versions[entity.id()] << std::endl;
                std::cout << "typeid()" << typeid(T).name() << std::endl;
            }
        }

        static T& get(size_t index=0)
        {
            return Static ? _ts[0] : _ts[index];
        }

        static uint8_t version(size_t index=0)
        {
            return Static ? _versions[0] : _versions[index];
        }

        static bool compare(Entity& entity, const uint8_t& version)
        {
            return _versions[entity.id()] == version;
        }

        static void refreshAll()
        {
            for(auto i{0}; i < MAX_ENTITIES; ++i)
            {
                refresh(i);
            }
        }

        static void refresh(size_t index=0)
        {
            ++_versions[index];
            std::cout << "component#" << _number << " version=" << (int)_versions[index] << std::endl;
        }


        static unsigned int _number;
        static uint32_t _bitSignature;
        static T _ts[];
        static uint8_t _versions[]; // Version of this component for a specific entity.
    };

    template <typename... T>
    void attach(Entity& entity)
    {
        (T::attach(entity), ...); // fold expression
    }

    /*template <class T, uint32_t U>
    std::vector<T> Component<T,U>::_ts{};*/
    template <class T, uint32_t Variant, bool Static>
    T Component<T,Variant,Static>::_ts[MAX_ENTITIES] = {};

    template <class T, uint32_t Variant, bool Static>
    uint8_t Component<T,Variant,Static>::_versions[MAX_ENTITIES] = {};

    template <class T, uint32_t Variant, bool Static>
    uint32_t Component<T,Variant,Static>::_number{_nextComponentNumber++};

    template <class T, uint32_t Variant, bool Static>
    uint32_t Component<T,Variant,Static>::_bitSignature{(uint32_t)(0x1 << _number)};
}