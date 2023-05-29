#pragma once

#include <iostream>

namespace lithium::json
{
    class Object
    {
    public:
        Object();

        virtual ~Object() noexcept;
    private:
    };

    std::istream& operator>>(std::istream& is, Object& obj);
}
