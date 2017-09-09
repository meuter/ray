#pragma once

#include <boost/operators.hpp>
#include <ray/math/Scalar.hpp>
#include <ostream>

namespace ray { namespace math {

    // TODO(cme): test this
    // TODO(cme): get rid of the bosst::brol thing
    template<typename scalar, size_t L, size_t C>
    class Matrix : 
                    boost::additive<Matrix<scalar,L,C>,
                    boost::additive2<Matrix<scalar,L,C>, scalar,
                    boost::multiplicative2<Matrix<scalar,L,C>, scalar,
                    boost::equality_comparable<Matrix<scalar,L,C> > > > >
    {
    public:
        scalar data[L*C];
        
        template<typename... scalars>
        Matrix(const scalars... x) : data{x...} {}

        const scalar &operator()(size_t l, size_t c) const { return reinterpret_cast<const scalar*>(this)[l*C+c]; }
        scalar &operator()(size_t l, size_t c)             { return reinterpret_cast<scalar*>(this)[l*C+c]; }

        constexpr size_t lines() const                { return L; }
        constexpr size_t columns() const              { return C; }

        bool operator==(const Matrix &r) const
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    if (!scalar_equal((*this)(l,c), r(l,c)) )
                        return false;
            return true;
        }

        Matrix &operator+=(const Matrix &r)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) += r(l,c);
            return (*this);
        }

        Matrix &operator-=(const Matrix &r)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) -= r(l,c);
            return (*this);
        }

        Matrix &operator+=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) += s;
            return (*this);
        }

        Matrix &operator-=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) -= s;
            return (*this);
        }

        Matrix &operator*=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) *= s;
            return (*this);
        }

        Matrix &operator/=(const scalar &s)
        {
            for (size_t l = 0; l < lines(); ++l)
                for (size_t c = 0; c < columns(); ++c)
                    (*this)(l,c) /= s;
            return (*this);
        }

        Matrix operator-() const
        {
            return -1 * (*this);
        }

        template<size_t M>
        Matrix<scalar,L,M> operator*(const Matrix<scalar,C,M> &r) const
        {
            Matrix<scalar,L,M> result;
            for (size_t i = 0; i < result.lines(); ++i)
            {
                for (size_t j = 0; j < result.columns(); ++j)
                {
                    result(i,j) = 0;
                    for (size_t k = 0; k < columns(); ++k)
                        result(i,j) += (*this)(i,k) * r(k, j);
                }
            }
            return result;
        }

        Matrix &operator*=(const Matrix &r)
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

    template<typename scalar, size_t N>
    scalar trace(const Matrix<scalar,N,N> &m)  
    {
        scalar accumulator = static_cast<scalar>(0.0);
        for (size_t i = 0; i < N; ++i)
            accumulator += m(i,i);
        return accumulator;
    }

    template<typename scalar, size_t L, size_t C>
    Matrix<scalar,C,L> transposed(const Matrix<scalar,L,C> &m)
    {
        Matrix<scalar,C,L> result;
        for (size_t l = 0; l < L; ++l)
            for (size_t c = 0; c < C; ++c)
                result(c,l) = m(l,c);
        return result;
    }
}}

