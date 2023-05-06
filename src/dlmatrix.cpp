#include "dlmatrix.h"

#include <random>
#include <algorithm>
#include <numeric>


lithium::Matrix::Matrix()
{

}

lithium::Matrix::Matrix(size_t rows, size_t columns, Real val) : _rows(rows, Vector(columns, val))
{
}

lithium::Matrix::Matrix(const std::initializer_list<Vector>& data) : _rows(data)
{

}

lithium::Matrix::~Matrix() noexcept
{

}

lithium::Matrix::Matrix(const lithium::Matrix& other) : _rows(other._rows)
{

}

lithium::Matrix::Matrix(lithium::Matrix&& other) noexcept : _rows(std::move(other._rows))
{

}

lithium::Matrix& lithium::Matrix::operator=(const lithium::Matrix& other)
{
    _rows = other._rows;
    return *this;
}

lithium::Matrix& lithium::Matrix::operator=(lithium::Matrix&& other) noexcept
{
    _rows = std::move(other._rows);
    return *this;
}


lithium::Matrix lithium::Matrix::operator+(const lithium::Matrix& other) const
{
    checkCompatibility(other);
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), result._rows.begin(), std::plus<Vector>());
    return result;
}

lithium::Matrix lithium::Matrix::operator-(const lithium::Matrix& other) const
{
    checkCompatibility(other);
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), result._rows.begin(), std::minus<Vector>());
    return result;
}

lithium::Matrix lithium::Matrix::operator*(Real scalar) const
{
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), result._rows.begin(), [scalar](const Vector& row) {
        return row * scalar;
    });
    return result;
}

lithium::Matrix lithium::Matrix::operator*(const lithium::Matrix& other) const
{
    checkCompatibility(other);
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), result._rows.begin(), [](const Vector& row1, const Vector& row2) {
        return row1 * row2;
    });
    return result;
}

lithium::Matrix lithium::Matrix::operator*(const lithium::Vector& other) const
{
    if (_rows.size() != other.size()) {
        throw std::length_error("lithium::Matrix and lithium::Vector must have the same size.");
    }
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), result.begin(), [&other](const Vector& row) {
        return row * other;
    });
    return result;
}

lithium::Matrix lithium::Matrix::operator/(Real scalar) const
{
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), result._rows.begin(), [scalar](const Vector& row) {
        return row / scalar;
    });
    return result;
}

lithium::Matrix lithium::Matrix::operator/(const lithium::Matrix& other) const
{
    checkCompatibility(other);
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), result._rows.begin(), [](const Vector& row1, const Vector& row2) {
        return row1 / row2;
    });
    return result;
}

lithium::Matrix lithium::Matrix::operator/(const lithium::Vector& other) const
{
    if (_rows.size() != other.size()) {
        throw std::length_error("lithium::Matrix and lithium::Vector must have the same size.");
    }
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), result.begin(), [&other](const Vector& row) {
        return row / other;
    });
    return result;
}

lithium::Matrix lithium::Matrix::operator+(const lithium::Vector& other) const
{
    if (_rows.size() != other.size()) {
        throw std::length_error("lithium::Matrix and lithium::Vector must have the same size.");
    }
    lithium::Matrix result(_rows.size(), _rows[0].size());
    std::transform(_rows.begin(), _rows.end(), other.begin(), result.begin(), std::plus<Vector>());
    return result;
}

lithium::Matrix lithium::Matrix::operator+=(const lithium::Matrix& other)
{
    checkCompatibility(other);
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), _rows.begin(), std::plus<Vector>());
    return *this;
}

lithium::Matrix lithium::Matrix::operator-=(const lithium::Matrix& other)
{
    checkCompatibility(other);
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), _rows.begin(), std::minus<Vector>());
    return *this;
}

lithium::Matrix lithium::Matrix::operator*=(Real scalar)
{
    std::transform(_rows.begin(), _rows.end(), _rows.begin(), [scalar](const Vector& row) {
        return row * scalar;
    });
    return *this;
}

lithium::Matrix lithium::Matrix::operator*=(const lithium::Matrix& other)
{
    checkCompatibility(other);
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), _rows.begin(), [](const Vector& row1, const Vector& row2) {
        return row1 * row2;
    });
    return *this;
}

lithium::Matrix lithium::Matrix::operator/=(Real scalar)
{
    std::transform(_rows.begin(), _rows.end(), _rows.begin(), [scalar](const Vector& row) {
        return row / scalar;
    });
    return *this;
}

lithium::Matrix lithium::Matrix::operator/=(const lithium::Matrix& other)
{
    checkCompatibility(other);
    std::transform(_rows.begin(), _rows.end(), other._rows.begin(), _rows.begin(), [](const Vector& row1, const Vector& row2) {
        return row1 / row2;
    });
    return *this;
}

lithium::Vector& lithium::Matrix::operator[](size_t index)
{
    return _rows[index];
}

const lithium::Vector& lithium::Matrix::operator[](size_t index) const
{
    return _rows[index];
}

Real lithium::Matrix::sum() const
{
    Real result = 0;
    for (const auto& row : _rows)
    {
        result += row.sum();
    }
    return result;
}

lithium::Vector& lithium::Matrix::at(size_t index)
{
    return _rows.at(index);
}

std::vector<lithium::Vector>::iterator lithium::Matrix::begin()
{
    return _rows.begin();
}

std::vector<lithium::Vector>::const_iterator lithium::Matrix::begin() const
{
    return _rows.begin();
}

std::vector<lithium::Vector>::iterator lithium::Matrix::end()
{
    return _rows.end();
}

std::vector<lithium::Vector>::const_iterator lithium::Matrix::end() const
{
    return _rows.end();
}

void lithium::Matrix::checkCompatibility(const lithium::Matrix& other) const
{
    if (_rows.size() != other._rows.size())
    {
        throw std::length_error("lithium::Matrix must have the same number of rows.");
    }
    if (_rows[0].size() != other._rows[0].size())
    {
        throw std::length_error("lithium::Matrix must have the same number of columns.");
    }
}

void lithium::Matrix::checkCompatibility(const lithium::Vector& other) const
{
    if (_rows[0].size() != other.size())
    {
        throw std::length_error("lithium::Matrix and lithium::Vector must have the same size.");
    }
}

lithium::Vector lithium::Matrix::row(size_t index) const
{
    return _rows[index];
}

lithium::Vector lithium::Matrix::column(size_t index) const
{
    lithium::Vector result(_rows.size());
    for (size_t i = 0; i < _rows.size(); ++i)
    {
        result[i] = _rows[i][index];
    }
    return result;
}

lithium::Matrix lithium::Matrix::dot(const lithium::Matrix& other) const
{
    //handle matrix dot product of different sizes for eg 4x2 and 2x3
    if (_rows[0].size() != other._rows.size())
    {
        throw std::length_error("lithium::Matrix must have the same number of columns as the other matrix's rows.");
    }
    lithium::Matrix result(_rows.size(), other._rows[0].size());
    for (size_t i = 0; i < _rows.size(); ++i)
    {
        for (size_t j = 0; j < other._rows[0].size(); ++j)
        {
            result[i][j] = _rows[i].dot(other.column(j));
        }
    }
    return result;
}

lithium::Vector lithium::Matrix::dot(const lithium::Vector& other) const
{
    checkCompatibility(other);
    lithium::Vector result(_rows.size());
    for (size_t i = 0; i < _rows.size(); ++i)
    {
        result[i] = _rows[i].dot(other);
    }
    return result;
}

lithium::Matrix lithium::Matrix::identity(size_t size, Real value)
{
    lithium::Matrix result(size, size);
    for (size_t i = 0; i < size; ++i)
    {
        result[i][i] = value;
    }
    return result;
}

lithium::Matrix lithium::Matrix::uniform(size_t rows, size_t columns, float a, float b)
{
    lithium::Matrix result(rows, columns);
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < columns; ++j)
        {
            result[i][j] = dis(gen);
        }
    }
    return result;
}

lithium::Matrix lithium::Matrix::zeros(size_t rows, size_t columns)
{
    return lithium::Matrix(rows, columns, 0.0f);
}

lithium::Matrix lithium::Matrix::transpose() const
{
    lithium::Matrix result(_rows[0].size(), _rows.size());
    for (size_t i = 0; i < _rows.size(); ++i)
    {
        for (size_t j = 0; j < _rows[0].size(); ++j)
        {
            result._rows[j][i] = _rows[i][j];
        }
    }
    return result;
}
