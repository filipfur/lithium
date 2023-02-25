#pragma once

namespace ecs
{
    class Entity
    {
    public:
        Entity()
        {
            _id = nextId++;
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

        bool hasComponents(uint32_t component) const
        {
            return (_componentMask & component) == component;
        }

        uint32_t id() const
        {
            return _id;
        }

    private:
        uint32_t _componentMask{0};
        uint32_t _id;
        static uint32_t nextId;
    };
}