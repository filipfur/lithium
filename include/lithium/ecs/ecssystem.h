#pragma once

#include <typeinfo>
#include <iostream>

#include "ecscomponent.h"
#include "ecsentity.h"
#include "ecsconstants.h"
#include "ecsslice.h"

namespace ecs
{

    template <class... T>
    class System : public Slice<T...>
    { 
        public:
            System() : _versions{}
            {

            }

            void update(std::set<Entity*>& entities,
                std::function<void(ecs::Entity&, typename T::value_type&...)> callback)
            {
                auto mask = Slice<T...>::mask();
                for(auto ptr : entities)
                {
                    const uint32_t entityId = ptr->id();
                    if(ptr->hasComponents(mask))
                    {
                        //bool test = ((_versions[entityId][T::_number] != T::version(entityId)) && ...);
                        Entity& entity = *ptr;
                        bool test = (T::compare(entity, _versions[entityId][T::_number]) && ...);
#ifdef ECS_TRACE
                        //std::cout << "ecs::System: Testing mask=" << mask << ", entity=" << entityId << ": " << (test ? "[ ]" : "[X]") << std::endl;
#endif
                        if(!test) // Check if NOT all versions match
                        {
#ifdef ECS_TRACE
                            std::cout << "ecs::System: Updating mask=" << mask << ", entity=" << entityId << std::endl;
#endif
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
            uint8_t _versions[ecs::GLOBAL_MAX_ENTITIES][ecs::GLOBAL_MAX_COMPONENTS];
    };
}