#pragma once

#include <typeinfo>
#include <functional>
#include <iostream>

#include "ecscomponent.h"
#include "ecsentity.h"

namespace ecs
{
    template <class... T>
    class System
    { 
        public:
            System()
            {
                _mask = (T::_id + ...);
            }

            void update(const std::vector<Entity>& entities,
                std::function<void(const ecs::Entity*, typename T::value_type&...)> callback) const
            {
                for(auto it = entities.begin(); it != entities.end(); ++it)
                {
                    if(it->hasComponents(_mask))
                    {
                        (callback(&(*it),
                            T::_ts[it->id()]...
                        )); 
                    }
                }
            }

            virtual ~System() noexcept
            {

            }

        private:
            uint32_t _mask{0};
    };
}