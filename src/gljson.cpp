#include "gljson.h"

lithium::json::Object::Object()
{

}

lithium::json::Object::~Object() noexcept
{

}

void assertKey(const std::string& key)
{
    if(key.length() <= 0)
    {
        throw std::runtime_error("Key is empty.");        
    }
}

void lithium::json::Object::parse(std::istream& is, lithium::json::Object& obj)
{
    char c;
    char startchar{0};
    char endchar{0};
    bool parseString{false};
    std::string stringValue{""};

    auto handleStart = [&](char start, char end) {
        if(startchar == 0)
        {
            startchar = start;
            endchar = end;
        }
        else
        {
            lithium::json::Object anotherOne;
            anotherOne._parent = &obj;
            obj.parse(is, anotherOne);
            obj._children.push_back(anotherOne);
        }
    };

    while((c = is.peek()) != EOF && !is.fail())
    {
        //std::cout << c;
        if(parseString)
        {
            if(c == '"')
            {
                parseString = false;
                std::cout << "String: " << stringValue << std::endl;
            }
            else
            {
                stringValue += c;
            }
        }
        else
        {
            switch(c)
            {
                case '{':
                    handleStart(c, '}');
                    break;
                case '[':
                    handleStart(c, ']');
                    break;
                case '"':
                    parseString = true;
                    break;
                case ':':
                    assertKey(stringValue);
                    break;
                case '}':
                case ']':
                    if(endchar != c)
                    {
                        throw std::runtime_error("Unexpected endchar.");
                    }
                    is.get();
                    return;
                    break;
                case '\n':
                case '\r':
                case '\t':
                case ' ':
                    break;
                default:
                    throw std::runtime_error("Unexpected character.");
                    break;
            }
        }
        is.get();
    }
}

std::istream& lithium::json::operator>>(std::istream& is, lithium::json::Object& obj)
{
    try
    {
        obj.parse(is, obj);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return is;
}