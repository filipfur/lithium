#pragma once

#include <iostream>
#include <unordered_map>

namespace lithium::json
{
    class Object
    {
    public:
        Object();

        virtual ~Object() noexcept;
    
        void add(const std::string& key, Object& obj)
        {
            obj._parent = this;
            _children.emplace(key, obj);
        }

        void add(const std::string& key, const std::string& value)
        {
            _values.emplace(key, value);
        }

        Object(const Object& other) : _parent{other._parent}, _children{other._children}
        {

        }

        Object(Object&& other) : _parent{other._parent}, _children{std::move(other._children)}
        {

        }

        Object& operator=(const Object& other)
        {
            _parent = other._parent;
            _children = other._children;
            return *this;
        }

        Object& operator=(Object&& other)
        {
            _parent = other._parent;
            _children = std::move(other._children);
            return *this;
        }

    private:
        void parse(std::istream& is, Object& obj);

        friend std::istream& operator>>(std::istream& is, Object& obj);

        Object* _parent{nullptr};
        std::unordered_map<std::string, Object> _children;
        std::unordered_map<std::string, std::string> _values;
    };
}
