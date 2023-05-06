#pragma once

#include <vector>
#include <ostream>

#include "dlreal.h"

namespace lithium
{
    class Vector
    {
    public:
        Vector(size_t dim, Real val=0.0);

        template <typename... Args>
        Vector(Real real, Args...args)
        {
            _data.insert(_data.end(), {real, args...});
        }

        Vector(const std::vector<Real>& data);
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
            std::copy(vector.begin(), vector.end(), std::ostream_iterator<Real>(os, " "));
            os << ']';
            return os;
        }

    private:
        Vector() = default;

        std::vector<Real> _data;
    };
}

