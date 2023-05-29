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
    std::string key{""};
    bool parseValue{false};

    auto handleStart = [&](char start, char end) {
        if(startchar == 0)
        {
            startchar = start;
            endchar = end;
        }
        else
        {
            lithium::json::Object anotherOne;
            obj.parse(is, anotherOne);
            obj.add(key, anotherOne);
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
                if(key.length() > 0)
                {
                    obj.add(key, stringValue);
                    key = "";
                }
                else
                {
                    key = stringValue;
                }
                stringValue = "";
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
                    assertKey(key);
                    parseValue = true;
                    break;
                case ',':
                    if(parseValue)
                    {
                        assertKey(key);
                        obj.add(key, stringValue);
                        std::cout << "Value: " << stringValue << std::endl;
                        stringValue = "";
                        parseValue = false;
                    }
                case '}':
                case ']':
                    if(endchar != c)
                    {
                        throw std::runtime_error("Unexpected endchar.");
                    }
                    if(parseValue)
                    {
                        assertKey(key);
                        obj.add(key, stringValue);
                        std::cout << "Value: " << stringValue << std::endl;
                        stringValue = "";
                        parseValue = false;
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
                    if(parseValue)
                    {
                        stringValue += c;
                    }
                    else
                    {
                        throw std::runtime_error("Unexpected character.");
                    }
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