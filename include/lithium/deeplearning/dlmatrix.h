#pragma once

#include "dlvector.h"

namespace lithium
{
    class Matrix
    {
    public:
        Matrix();
        explicit Matrix(size_t rows, size_t columns, Real val=0.0);
        //explicit Matrix(const std::vector<Vector>& data);
        /*template <typename... Args>
        explicit Matrix(const lithium::Vector& vec, Args... args) : Matrix({vec, args...}) {}*/
        explicit Matrix(const std::initializer_list<lithium::Vector>& init_list);

        virtual ~Matrix() noexcept;

        Matrix(const Matrix& other);
        Matrix(Matrix&& other) noexcept;
        Matrix& operator=(const Matrix& other);
        Matrix& operator=(Matrix&& other) noexcept;

        Matrix operator+(const Matrix& other) const;
        Matrix operator-(const Matrix& other) const;
        Matrix operator*(Real scalar) const;
        Matrix operator*(const Matrix& other) const;
        Matrix operator/(Real scalar) const;
        Matrix operator/(const Matrix& other) const;

        Matrix operator+(const Vector& other) const;
        Matrix operator-(const Vector& other) const;
        Matrix operator*(const Vector& other) const;
        Matrix operator/(const Vector& other) const;

        Matrix operator+=(const Matrix& other);
        Matrix operator-=(const Matrix& other);
        Matrix operator*=(Real scalar);
        Matrix operator*=(const Matrix& other);
        Matrix operator/=(Real scalar);
        Matrix operator/=(const Matrix& other);


        Vector& operator[](size_t index);
        const Vector& operator[](size_t index) const;
        Vector& at(size_t index);

        Matrix transpose() const;

        std::vector<Vector>::iterator begin();
        std::vector<Vector>::iterator end();
        std::vector<Vector>::const_iterator begin() const;
        std::vector<Vector>::const_iterator end() const;

        bool operator==(const Matrix& other) const;
        bool operator!=(const Matrix& other) const;

        Real sum() const;
        Real mean() const;
        Real max() const;
        Real min() const;

        Matrix dot(const Matrix& other) const;
        Vector dot(const Vector& other) const;
        static Matrix identity(size_t size, Real val=1.0);
        static Matrix uniform(size_t rows, size_t columns, float a=0.0f, float b=1.0f);
        static Matrix zeros(size_t rows, size_t columns);
        Vector row(size_t index) const;
        Vector column(size_t index) const;

        void checkCompatibility(const Matrix& other) const;
        void checkCompatibility(const Vector& other) const;

        size_t size() const
        {
            return _rows.size();
        }

        size_t rows() const
        {
            return size();
        }

        size_t columns() const
        {
            return _rows.size() > 0 ? _rows[0].size() : 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
        {
            os << "mat" << matrix.rows() << 'x' << matrix.columns() << "[";
            if(matrix.rows() > 1)
            {
                os << std::endl;
            }
            for (size_t i = 0; i < matrix._rows.size(); ++i)
            {
                os << matrix._rows[i];
                if (i < matrix._rows.size() - 1)
                {
                    os << std::endl;
                }
                if(i == 32)
                {
                    os << "...";
                    break;
                }
            }
            os << "]";
            return os;
        }

    private:
        std::vector<Vector> _rows;
    };
}