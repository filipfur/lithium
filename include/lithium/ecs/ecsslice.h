#pragma once

#include <functional>

namespace ecs
{
    template<class... T>
    class Slice
    {
    public:
        using EntityContainerType = std::set<ecs::Entity*>;
        
        static void forEach(EntityContainerType& entities,
            std::function<void(ecs::Entity&, typename T::value_type&...)> callback)
        {
            auto mask = Slice<T...>::mask();
            for(auto ptr : entities)
            {
                const uint32_t entityId = ptr->id();
                if(ptr->hasComponents(mask))
                {
                    Entity& entity = *ptr;
                    (callback(entity, T::get(entityId)...));
                }
            }
        }

        static uint32_t mask()
        {
            return (T::bitSignature() + ...);
        }
    };
}