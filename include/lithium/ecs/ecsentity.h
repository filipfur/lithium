#pragma once

namespace ecs
{
    class Entity
    {
    public:
        Entity() : _id{nextId++}, _componentMask{0}
        {
        }

        virtual ~Entity() noexcept
        {
        }

        Entity(const Entity& other) : _id{other._id}, _componentMask{other._componentMask}
        {
        }

        Entity(Entity&& other) : _id{other._id}, _componentMask{other._componentMask}
        {
        }

        Entity& operator=(const Entity& other)
        {
            _id = other._id;
            _componentMask = other._componentMask;
            return *this;
        }

        Entity& operator=(Entity&& other)
        {
            _id = other._id;
            _componentMask = other._componentMask;
            return *this;
        }

        void addComponent(uint32_t componentId)
        {
            _componentMask |= componentId;
        }

        void removeComponent(uint32_t componentId)
        {
            _componentMask &= ~componentId;
        }
        
        bool hasComponents(uint32_t component) const
        {
            return (_componentMask & component) == component;
        }

        template <typename T>
        typename T::value_type* get() const
        {
            if(!hasComponents(T::bitSignature()))
            {
                return nullptr;
            }
            return &T::get(_id);
        }

        template <typename T>
        void set(typename T::value_type t)
        {
            T::set(t, _id);
        }

        uint32_t id() const
        {
            return _id;
        }

    private:
        uint32_t _id;
        uint32_t _componentMask{0};
        static uint32_t nextId;
    };
}