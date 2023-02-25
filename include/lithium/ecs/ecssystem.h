#pragma once

#include <typeinfo>
#include <functional>

namespace ecs
{
    class System
    { 
        public:

            System()
            {
            }

            virtual ~System() noexcept
            {

            }

        private:

            uint32_t mask{0};
    };
}