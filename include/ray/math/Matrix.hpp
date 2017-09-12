#pragma once

#include <ray/math/Scalar.hpp>
#include <initializer_list>
#include <array>
#include <ostream>
#include <algorithm>

namespace ray { namespace math {

    template<typename T, size_t L, size_t C>
    class Matrix 
    {
        using scalar = T;
    public:
        std::array<scalar, L*C> data;
        
        constexpr Matrix() = default;

        template<typename... scalars>
        constexpr Matrix(const scalars... x) : data{x...} {}

        const scalar &operator()(size_t l, size_t c) const { return reinterpret_cast<const scalar*>(this)[l*C+c]; }
        scalar &operator()(size_t l, size_t c)             { return reinterpret_cast<scalar*>(this)[l*C+c]; }

        constexpr size_t lines() const                { return L; }
        constexpr size_t columns() const              { return C; }

        template<typename U>
        constexpr Matrix &operator+=(const Matrix<U,L,C> &r)
        {
            for (size_t l = 0; l < L; ++l)
                for (size_t c = 0; c < C; ++c)
                    (*this)(l,c) += r(l,c);
            return (*this);
        }

        template<typename U>
        constexpr Matrix &operator-=(const Matrix<U,L,C> &r)
        {
            for (size_t l = 0; l < L; ++l)
                for (size_t c = 0; c < C; ++c)
                    (*this)(l,c) -= r(l,c);
            return (*this);
        }

        template<typename U>
        constexpr Matrix &operator*=(const Scalar<U> &s)
        {
            for (size_t l = 0; l < L; ++l)
                for (size_t c = 0; c < C; ++c)
                    (*this)(l,c) *= s;
            return (*this);
        }

        template<typename U>
        constexpr Matrix &operator/=(const Scalar<U> &s)
        {
            for (size_t l = 0; l < L; ++l)
                for (size_t c = 0; c < C; ++c)
                    (*this)(l,c) /= s;
            return (*this);
        }

        constexpr Matrix operator-() const
        {
            return -1 * (*this);
        }
              
        template<typename U>
        Matrix &operator*=(const Matrix<U,C,L> &r)
        {
            (*this) = (*this) * r;
            return *this;
        }
    };

    template<typename scalar, size_t L, size_t C>
    std::ostream &operator<<(std::ostream &out, const Matrix<scalar,L,C> &m)
    {
        size_t l, c;
        for (l = 0; l < L; ++l)
        {
            for (c = 0; c < C; ++c)
                out << m(l,c) << " ";
            out << std::endl;
        }
        return out;
    } 

    template<typename T, size_t N>
    constexpr auto trace(const Matrix<T,N,N> &m)  
    {
        auto result = T{0};
        for (size_t i = 0; i < N; ++i)
            result += m(i,i);
        return result;
    }

    template<typename scalar, size_t L, size_t C>
    auto transpose(const Matrix<scalar,L,C> &m)
    {
        Matrix<scalar,C,L> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(c,l) = m(l,c);
        return result;
    }

    template<typename T, typename U, size_t L, size_t C> 
    constexpr bool operator==(const Matrix<T,L,C> &a, const Matrix<U,L,C> &b) 
    {
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                if (!scalar_equal(a(l,c), b(l,c)) )
                    return false;
        return true;
    }

    template<typename T, typename U, size_t L, size_t C> 
    constexpr auto operator!=(const Matrix<T,L,C> &a, const Matrix<U,L,C> &b) { return !(a == b); }

    template<typename T, typename U, size_t L, size_t C> 
    auto operator+(const Matrix<T,L,C> &a, const Matrix<U,L,C> &b) 
    {
        Matrix<widest<T,U>,L,C> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(l,c) = a(l,c)+b(l,c);
        return result;
    }

    template<typename T, typename U, size_t L, size_t C> 
    auto operator-(const Matrix<T,L,C> &a, const Matrix<U,L,C> &b) 
    {
        Matrix<widest<T,U>,L,C> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(l,c) = a(l,c)-b(l,c);
        return result;
    }

    template<typename T, typename U, size_t L, size_t C> 
    auto operator*(const Matrix<T,L,C> &a, const Scalar<U> &f) 
    {
        Matrix<widest<T,U>,L,C> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(l,c) = a(l,c) * f;
        return result;
    }

    template<typename T, typename U, size_t L, size_t C> 
    auto operator*(const Scalar<U> &f, const Matrix<T,L,C> &a) { return (a*f); }

    template<typename T, typename U, size_t L, size_t C> 
    auto operator/(const Matrix<T,L,C> &a, const Scalar<U> &d) 
    {
        Matrix<widest<T,U>,L,C> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(l,c) = a(l,c) / d;
        return result;
    }

    template<typename T, typename U, size_t L, size_t C> 
    auto operator/(const Scalar<T> &d, const Matrix<U,L,C> &a) 
    {
        Matrix<widest<T,U>,L,C> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(l,c) = d / a(l,c);
        return result;
    }


    template<typename T, typename U, size_t L, size_t M, size_t C>
    auto operator*(const Matrix<T,L,M> &a, const Matrix<U,M,C> &b)
    {
        Matrix<widest<T,U>,L,C> result;
        for (size_t l = 0; l < L; ++l)
        {
            for (size_t c = 0; c < C; ++c)
            {
                result(l,c) = 0;
                for (size_t m = 0; m < M; ++m)
                    result(l,c) += a(l,m) * b(m,c);
            }
        }
        return result;
    }


}}

