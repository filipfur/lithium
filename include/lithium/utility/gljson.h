#pragma once

#include <iostream>
#include <vector>

namespace lithium::json
{
    class Object
    {
    public:
        Object();

        virtual ~Object() noexcept;
    private:
        void parse(std::istream& is, Object& obj);

        friend std::istream& operator>>(std::istream& is, Object& obj);

        Object(const Object& other) = delete;

        Object(Object&& other) = delete;

        Object& operator=(const Object& other) = delete;

        Object& operator=(Object&& other) = delete;

        Object* _parent{nullptr};
        std::vector<Object> _children;
    };
}
