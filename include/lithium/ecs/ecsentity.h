#pragma once

namespace ecs
{
    class Entity
    {
    public:
        Entity()
        {

        }

        virtual ~Entity() noexcept
        {

        }

        void addComponent(uint32_t component)
        {
            _componentMask |= component;
        }

        void removeComponent(uint32_t component)
        {
            _componentMask &= ~component;
        }

        bool hasComponents(uint32_t component)
        {
            return (_componentMask & component) == component;
        }

    private:
        uint32_t _componentMask{0};

    };
}