#include "dlvector.h"

#include <functional>
#include <algorithm>
#include <numeric>

lithium::Vector::Vector(size_t dim, Real val) : _data(dim, val)
{
}

lithium::Vector::Vector(const std::vector<Real>& data) : _data(data) 
{

}

lithium::Vector::~Vector() noexcept
{

}

lithium::Vector::Vector(const lithium::Vector& other) : _data(other._data)
{

}

lithium::Vector::Vector(lithium::Vector&& other) noexcept : _data(std::move(other._data))
{

}

lithium::Vector& lithium::Vector::operator=(const lithium::Vector& other)
{
    _data = other._data;
    return *this;
}

lithium::Vector& lithium::Vector::operator=(lithium::Vector&& other) noexcept
{
    _data = std::move(other._data);
    return *this;
}

lithium::Vector lithium::Vector::operator+(const lithium::Vector& other) const {
    if (_data.size() != other._data.size()) {
        throw std::length_error("lithium::Vectors must have the same size.");
    }
    lithium::Vector result;
    result._data.resize(_data.size());
    std::transform(_data.begin(), _data.end(), other._data.begin(), result._data.begin(), std::plus<Real>());
    return result;
}

lithium::Vector lithium::Vector::operator-(const lithium::Vector& other) const
{
    checkCompatibility(other);
    lithium::Vector result;
    result._data.resize(_data.size());
    std::transform(_data.begin(), _data.end(), other._data.begin(), result._data.begin(), std::minus<Real>());
    return result;
}

lithium::Vector lithium::Vector::operator*(Real scalar) const
{
    lithium::Vector result(_data.size());
    std::transform(_data.begin(), _data.end(), result._data.begin(), [scalar](Real val) { return scalar * val; });
    return result;
}

lithium::Vector lithium::Vector::operator*(const lithium::Vector& other) const
{
    checkCompatibility(other);
    lithium::Vector result;
    result._data.resize(_data.size());
    std::transform(_data.begin(), _data.end(), other._data.begin(), result._data.begin(), std::multiplies<Real>());
    return result;
}

lithium::Vector lithium::Vector::operator/(Real scalar) const
{
    lithium::Vector result(_data.size());
    std::transform(_data.begin(), _data.end(), result._data.begin(), [scalar](Real val) { return val / scalar; });
    return result;
}

lithium::Vector lithium::Vector::operator/(const lithium::Vector& other) const
{
    checkCompatibility(other);
    lithium::Vector result(_data.size());
    std::transform(_data.begin(), _data.end(), other._data.begin(), result._data.begin(), std::divides<Real>());
    return result;
}

bool lithium::Vector::operator==(const Vector& other) const
{
    return _data == other._data;
}

bool lithium::Vector::operator!=(const Vector& other) const
{
    return !(*this == other);
}

Real lithium::Vector::sum() const
{
    return std::accumulate(_data.begin(), _data.end(), 0.0);
}

Real lithium::Vector::mean() const
{
    return sum() / _data.size();
}

Real lithium::Vector::max() const
{
    return *std::max_element(_data.begin(), _data.end());
}

Real lithium::Vector::min() const
{
    return *std::min_element(_data.begin(), _data.end());
}

Real lithium::Vector::length() const
{
    return std::sqrt(length2());
}

Real lithium::Vector::length2() const
{
    return std::inner_product(_data.begin(), _data.end(), _data.begin(), 0.0);
}

Real lithium::Vector::distance(const Vector& other) const
{
    checkCompatibility(other);
    return (*this - other).length();
}

Real lithium::Vector::distance2(const Vector& other) const
{
    checkCompatibility(other);
    return (*this - other).length2();
}

lithium::Vector lithium::Vector::normalized() const
{
    return *this / length();
}

lithium::Vector& lithium::Vector::normalize()
{
    *this /= length();
    return *this;
}

lithium::Vector& lithium::Vector::operator+=(Real scalar)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [scalar](Real val) { return scalar + val; });
    return *this;
}

lithium::Vector& lithium::Vector::operator+=(const Vector& other)
{
    checkCompatibility(other);
    std::transform(_data.begin(), _data.end(), other._data.begin(), _data.begin(), std::plus<Real>());
    return *this;
}

lithium::Vector& lithium::Vector::operator-=(Real scalar)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [scalar](Real val) { return val - scalar; });
    return *this;
}

lithium::Vector& lithium::Vector::operator-=(const Vector& other)
{
    checkCompatibility(other);
    std::transform(_data.begin(), _data.end(), other._data.begin(), _data.begin(), std::minus<Real>());
    return *this;
}

lithium::Vector& lithium::Vector::operator*=(Real scalar)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [scalar](Real val) { return scalar * val; });
    return *this;
}

lithium::Vector& lithium::Vector::operator*=(const Vector& other)
{
    checkCompatibility(other);
    std::transform(_data.begin(), _data.end(), other._data.begin(), _data.begin(), std::multiplies<Real>());
    return *this;
}

lithium::Vector& lithium::Vector::operator/=(Real scalar)
{
    std::transform(_data.begin(), _data.end(), _data.begin(), [scalar](Real val) { return val / scalar; });
    return *this;
}

lithium::Vector& lithium::Vector::operator/=(const Vector& other)
{
    checkCompatibility(other);
    std::transform(_data.begin(), _data.end(), other._data.begin(), _data.begin(), std::divides<Real>());
    return *this;
}

Real lithium::Vector::at(size_t index) const
{
    return _data.at(index);
}

Real& lithium::Vector::operator[](size_t index)
{
    return _data[index];
}

const Real& lithium::Vector::operator[](size_t index) const
{
    return _data[index];
}

Real lithium::Vector::dot(const Vector& other) const
{
    checkCompatibility(other);
    return std::inner_product(_data.begin(), _data.end(), other._data.begin(), 0.0);
}

void lithium::Vector::checkCompatibility(const Vector& other) const
{
    if (_data.size() != other._data.size())
    {
        throw std::length_error("lithium::Vectors must have the same size.");
    }
}