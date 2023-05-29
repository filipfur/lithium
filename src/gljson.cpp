#include "gljson.h"

lithium::json::Object::Object()
{

}

lithium::json::Object::~Object() noexcept
{

}

std::istream& lithium::json::operator>>(std::istream& is, lithium::json::Object& obj)
{
    char c;
    while(is >> c)
    {
        std::cout << c;
        std::cout << "hello" << std::endl;
    }
    return is;
}