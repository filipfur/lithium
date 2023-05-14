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
                _mask = (T::bitSignature() + ...);
            }

            void update(std::set<Entity*>& entities,
                std::function<bool(ecs::Entity&, typename T::value_type&...)> callback)
            {
                for(auto ptr : entities)
                {
                    const uint32_t entityId = ptr->id();
                    if(ptr->hasComponents(_mask))
                    {
                        //bool test = ((_versions[entityId][T::_number] != T::version(entityId)) && ...);
                        Entity& entity = *ptr;
                        bool test = (T::compare(entity, _versions[entityId][T::_number]) && ...);
#ifdef ECS_TRACE
                        //std::cout << "ecs::System: Testing mask=" << _mask << ", entity=" << entityId << ": " << (test ? "[ ]" : "[X]") << std::endl;
#endif
                        if(!test) // Check if NOT all versions match
                        {
#ifdef ECS_TRACE
                            std::cout << "ecs::System: Updating mask=" << _mask << ", entity=" << entityId << std::endl;
#endif
                            bool res = (callback(entity,
                                T::get(entityId)...
                            ));
                            (T::increment(entity, std::is_const_v<T> == false),...); // Increment version of NON-const components
                            (setVersion(entity, T::_number, T::version(entityId)), ...); // Update own version to match latest-greatest.
                            if(!res)
                            {
                                break;
                            }
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