#include "gljson.h"

lithium::json::Json::Json(const lithium::json::Type& type) : Json{type, "", ""}
{

}

lithium::json::Json::Json(const lithium::json::Type& type, const std::string& key, const std::string& value)
    : _type{type}, _key{key}, _value{value}
{

}

lithium::json::Json::~Json() noexcept
{

}

void assertKey(const std::string& key)
{
    if(key.length() <= 0)
    {
        throw std::runtime_error("Key is empty.");        
    }
}

enum class State
{
    ParseKey,
    ParseValue,
    ParseElements
};

void lithium::json::Json::parse(std::istream& is, lithium::json::Json& obj)
{
    char c;
    char startchar{0};
    char endchar{0};
    bool parseString{false};
    std::string stringValue{""};
    std::string key{""};
    State state{State::ParseKey};

    auto handleStart = [&](char start, char end) {
        if(startchar == 0)
        {
            if(startchar == '{')
            {
                state = State::ParseKey;
            }
            else if(startchar == '[')
            {
                state = State::ParseElements;
            }
            startchar = start;
            endchar = end;
        }
        else
        {
            lithium::json::Json anotherOne{start == '{' ? lithium::json::Object : lithium::json::Array, key, ""};
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
                switch(state)
                {
                    case State::ParseKey:
                        key = stringValue;
                        // state transition handled in ':' case
                        break;
                    case State::ParseValue:
                    {
                        assertKey(key);
                        Json strElem{lithium::json::Parameter, key, stringValue};
                        strElem._stringType = true;
                        obj.add(key, strElem);
                        key = "";
                        state = State::ParseKey;
                        break;
                    }
                    case State::ParseElements:
                    {
                        Json strElem{lithium::json::Element, "", stringValue};
                        strElem._stringType = true;
                        obj.insert(strElem);
                        break;
                    }
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
                    if(_type == lithium::json::Unassigned)
                    {
                        _type = lithium::json::Object;
                    }
                    handleStart(c, '}');
                    break;
                case '[':
                    if(_type == lithium::json::Unassigned)
                    {
                        _type = lithium::json::Array;
                    }
                    handleStart(c, ']');
                    break;
                case '"':
                    parseString = true;
                    break;
                case ':':
                    assertKey(key);
                    state = State::ParseValue;
                    break;
                case ',':
                    switch(state)
                    {
                        case State::ParseElements:
                        {
                            if(stringValue.size() <= 0) // Probably already handled in string parsing.
                            {
                                Json elementObj{lithium::json::Element, "", stringValue};
                                obj.insert(elementObj);
                                stringValue = "";
                            }
                            break;
                        }
                        case State::ParseValue:
                        {
                            assertKey(key);
                            Json valueObj{lithium::json::Parameter, key, stringValue};
                            obj.add(key, valueObj);
                            stringValue = "";
                            state = State::ParseKey;
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                case '}':
                case ']':
                    if(endchar != c)
                    {
                        throw std::runtime_error("Unexpected endchar.");
                    }
                    switch(state)
                    {
                        case State::ParseValue:
                            if(stringValue.size() > 0)
                            {
                                assertKey(key);
                                Json valueObj{lithium::json::Parameter, key, stringValue};
                                obj.add(key, valueObj);
                                stringValue = "";
                                state = State::ParseKey;
                            }
                            break;
                        case State::ParseElements:
                            if(stringValue.size() > 0)
                            {
                                Json elementObj{lithium::json::Element, "", stringValue};
                                obj.insert(elementObj);
                                stringValue = "";
                                state = State::ParseKey;
                            }
                            break;
                        case State::ParseKey:
                            throw std::runtime_error("Unexpected endchar during ParseKey.");
                            break;
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
                    switch(state)
                    {
                        case State::ParseValue:
                        case State::ParseElements:
                            stringValue += c;
                            break;
                        default:
                            throw std::runtime_error("Unexpected character.");
                            break;
                    }
                    break;
            }
        }
        is.get();
    }
}

std::istream& lithium::json::operator>>(std::istream& is, lithium::json::Json& obj)
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

void lithium::json::Json::print(std::ostream& os, std::string indent) const
{
    std::string delim{""};
    switch(_type)
    {
        case lithium::json::Object:
            os << indent << "{" << std::endl;
            for(auto& pair : _children)
            {
                os << delim << indent << "  \"" << pair.first << "\": ";
                pair.second.print(os, indent + "  ");
                delim = ",\n";
            }
            os << std::endl << indent << "}";
            break;
        case lithium::json::Array:
            os << indent << "[" << std::endl;
            for(auto& child : _array)
            {
                os << delim << indent;
                child.print(os, indent + "  ");
                delim = ",\n";
            }
            os << std::endl << indent << "]";
            break;
        case lithium::json::Parameter:
        case lithium::json::Element:
            if(_stringType)
            {
                os << '"' << _value << '"';
            }
            else
            {
                os << _value;
            }
            break;
        default:
            throw(std::runtime_error("Unknown type."));
            break;
    }
}

std::ostream& lithium::json::operator<<(std::ostream& os, const lithium::json::Json& obj)
{
    try
    {
        obj.print(os, "");
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}