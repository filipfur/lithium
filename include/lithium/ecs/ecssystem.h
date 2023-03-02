#pragma once

#include <typeinfo>
#include <functional>
#include <iostream>

#include "ecscomponent.h"
#include "ecsentity.h"
#include "ecsconstants.h"

namespace ecs
{

    template <class... T>
    class System
    { 
        public:
            System() : _versions{}
            {
                _mask = (T::_bitSignature + ...);
            }

            void update(std::vector<Entity>& entities,
                std::function<void(ecs::Entity&, typename T::value_type&...)> callback)
            {
                for(auto it = entities.begin(); it != entities.end(); ++it)
                {
                    const uint32_t entityId = it->id();
                    if(it->hasComponents(_mask))
                    {
                        //bool test = ((_versions[entityId][T::_number] != T::version(entityId)) && ...);
                        Entity& entity = *it;
                        bool test = (T::compare(entity, _versions[entityId][T::_number]) && ...);
                        if(!test) // Check if NOT all versions match
                        {
                            (callback(entity,
                                T::get(entityId)...
                            ));
                            (T::increment(entity, std::is_const_v<T> == false),...); // Increment version of NON-const components
                            (setVersion(entity, T::_number, T::version(entityId)), ...); // Update own version to match latest-greatest.
                        }
                    }
                }
            }

            void setVersion(Entity& entity, size_t componentNumber, uint8_t version)
            {
                _versions[entity.id()][componentNumber] = version;
            }

            virtual ~System() noexcept
            {

            }

        private:
            uint32_t _mask{0};
            uint8_t _versions[ecs::GLOBAL_MAX_ENTITIES][ecs::GLOBAL_MAX_COMPONENTS];
    };
}