#pragma once

#include <vector>
#include <ostream>
#include <iterator>

#include "dlreal.h"

namespace lithium
{
    class Vector
    {
    public:
        Vector(size_t dim, Real val=0.0);
        Vector(const std::vector<Real>& data);
        template <typename... Args>
        explicit Vector(Real real, Args...args) : Vector{std::vector<Real>{real, args...}} {}

        virtual ~Vector() noexcept;

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;
        Vector& operator=(const Vector& other);
        Vector& operator=(Vector&& other) noexcept;

        Vector operator+(const Vector& other) const;

        Vector operator-(const Vector& other) const;

        Vector operator*(Real scalar) const;

        Vector operator*(const Vector& other) const;

        Vector operator/(Real scalar) const;

        Vector operator/(const Vector& other) const;

        bool operator==(const Vector& other) const;

        bool operator!=(const Vector& other) const;

        Real sum() const;

        Real mean() const;

        Real max() const;

        Real min() const;

        Real length() const;

        Real length2() const;

        Real distance(const Vector& other) const;

        Real distance2(const Vector& other) const;

        Vector normalized() const;

        Vector& normalize();

        Vector& operator+=(Real scalar);

        Vector& operator+=(const Vector& other);

        Vector& operator-=(Real scalar);

        Vector& operator-=(const Vector& other);

        Vector& operator*=(Real scalar);

        Vector& operator*=(const Vector& other);

        Vector& operator/=(Real scalar);

        Vector& operator/=(const Vector& other);

        Real at(size_t index) const;

        Real& operator[](size_t index);
        const Real& operator[](size_t index) const;

        Real dot(const Vector& other) const;

        inline void checkCompatibility(const Vector& other) const;

        const std::vector<Real>& data() const { return _data; }

        std::vector<Real>::iterator begin() { return _data.begin(); }
        std::vector<Real>::iterator end() { return _data.end(); }
        std::vector<Real>::const_iterator begin() const { return _data.begin(); }
        std::vector<Real>::const_iterator end() const { return _data.end(); }

        size_t size() const { return _data.size(); }
        
        friend std::ostream& operator<<(std::ostream& os, const lithium::Vector& vector)
        {
            os << "vec" << vector.size() << "[ ";
            auto end = vector.end();
            if(vector.size() > 32)
            {
                end = vector.begin() + 32;
            }
            std::copy(vector.begin(), end, std::ostream_iterator<Real>(os, " "));
            if(vector.size() > 32)
            {
                os << "... ";
            }
            os << ']';
            return os;
        }

    private:
        Vector() = default;

        std::vector<Real> _data;
    };
}

