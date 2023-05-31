#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>

namespace lithium::json
{
    enum Type{
        Unassigned,
        Object,
        Parameter,
        Array,
        Element
    };

    class Json
    {
    public:
        Json(const Type& type=Unassigned);

        Json(const Type& type, const std::string& key, const std::string& value);

        virtual ~Json() noexcept;
    
        void add(const std::string& key, Json& obj);

        void insert(Json& obj);

        Json(const Json& other) : _type{other._type}, _key{other._key}, _value{other._value}, _parent{other._parent}, _children{other._children}, _array{other._array}, _stringType{other._stringType}
        {

        }

        Json(Json&& other) : _type{other._type}, _key{std::move(other._key)}, _value{std::move(other._value)}, _parent{other._parent}, _children{std::move(other._children)}, _array{std::move(other._array)}, _stringType{other._stringType}
        {

        }

        Json& operator=(const Json& other)
        {
            _type = other._type;
            _key = other._key;
            _value = other._value;
            _parent = other._parent;
            _children = other._children;
            _array = other._array;
            _stringType = other._stringType;
            return *this;
        }

        Json& operator=(Json&& other)
        {
            _type = other._type;
            _key = std::move(other._key);
            _value = std::move(other._value);
            _parent = other._parent;
            _children = std::move(other._children);
            _array = std::move(other._array);
            _stringType = other._stringType;
            return *this;
        }

        Json& operator[](const std::string& key)
        {
            if(!isObject())
            {
                throw std::runtime_error("trying to access parameter of a non-object");
            }
            return _children[key];
        }

        const Json& operator[](const std::string& key) const
        {
            if(!isObject())
            {
                throw std::runtime_error("trying to access parameter of a non-object");
            }
            return _children.at(key);
        }

        Json& operator[](size_t index)
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access element of a non-array");
            }
            return _array[index];
        }

        const Json& operator[](size_t index) const
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access element of a non-array");
            }
            return _array.at(index);
        }

        const Json& at(const std::string& key) const
        {
            if(!isObject())
            {
                throw std::runtime_error("trying to access parameter of a non-object");
            }
            return _children.at(key);
        }

        const Json& at(size_t index) const
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access element of a non-array");
            }
            return _array.at(index);
        }

        std::map<std::string,lithium::json::Json> children()
        {
            return _children;
        }

        bool contains(const std::string& key) const
        {
            if(!isObject())
            {
                throw std::runtime_error("trying to access parameter of a non-object");
            }
            return _children.find(key) != _children.end();
        }

        size_t size() const
        {
            if(_type == Type::Object)
            {
                return _children.size();
            }
            else if(_type == Type::Array)
            {
                return _array.size();
            }
            else
            {
                throw std::runtime_error("trying to get size of a non-object and non-array");
            }
        }

        std::vector<Json>::iterator begin()
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access begin iterator of a non-array");
            }
            return _array.begin();
        }

        std::vector<Json>::iterator end()
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access end iterator of a non-array");
            }
            return _array.end();
        }

        std::vector<Json>::const_iterator begin() const
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access begin const_iterator of a non-array");
            }
            return _array.begin();
        }

        std::vector<Json>::const_iterator end() const
        {
            if(!isArray())
            {
                throw std::runtime_error("trying to access end const_iterator of a non-array");
            }
            return _array.end();
        }

        bool isObject() const
        {
            return _type == Type::Object;
        }

        bool isArray() const
        {
            return _type == Type::Array;
        }

        bool isParameter() const
        {
            return _type == Type::Parameter;
        }

        bool isElement() const
        {
            return _type == Type::Element;
        }

        std::string key() const
        {
            return _key;
        }

        Type type() const
        {
            return _type;
        }

        std::string value() const
        {
            return _value;
        }

        bool toBool() const
        {
            return _value == "true" || _value == "1";
        }

        int toInt() const
        {
            return std::stoi(_value);
        }

        float toFloat() const
        {
            return std::stof(_value);
        }

        double toDouble() const
        {
            return std::stod(_value);
        }

        explicit operator bool() const
        {
            return toBool();
        }

        explicit operator int() const
        {
            return toInt();
        }

        explicit operator float() const
        {
            return toFloat();
        }

        explicit operator double() const
        {
            return toDouble();
        }

        operator std::string() const
        {
            return _value;
        }

        void clear()
        {
            _type = Type::Unassigned;
            _key.clear();
            _value.clear();
            _parent = nullptr;
            _children.clear();
            _array.clear();
            _stringType = false;
        }

        template <typename T>
        T get() const
        {
            if constexpr (std::is_same_v<T, bool>)
            {
                return toBool();
            }
            else if constexpr (std::is_same_v<T, int>)
            {
                return toInt();
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                return toFloat();
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return toDouble();
            }
            else if constexpr (std::is_same_v<T, std::string>)
            {
                return _value;
            }
            else
            {
                throw std::runtime_error("unsupported type");
            }
        }

    private:
        void parse(std::istream& is, Json& obj);
        void print(std::ostream& os, std::string indent) const;

        friend std::istream& operator>>(std::istream& is, Json& obj);
        friend std::ostream& operator<<(std::ostream& os, const Json& obj);

        Json* _parent{nullptr};
        std::map<std::string, Json> _children;
        std::vector<Json> _array;
        Type _type;
        std::string _key{""};
        std::string _value{""};
        bool _stringType{false};
    };
}
