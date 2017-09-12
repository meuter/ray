#pragma once

#include <type_traits>
#include <cstdint>
#include <limits>
#include <cmath>

namespace ray { namespace math {

    template<typename U>
    using floating_only = typename std::enable_if<std::is_floating_point<U>::value && !std::is_integral<U>::value, int>::type;

    template<typename U>
    using integral_only = typename std::enable_if<!std::is_floating_point<U>::value && std::is_integral<U>::value, int>::type;

    template<typename U>
    using scalar_only = typename std::enable_if<std::is_arithmetic<U>::value, int>::type;

    template<typename U, scalar_only<U> = 0>
    using Scalar = U;
    
    template<typename T, typename U>
    using widest = decltype(T{}*U{});
    
    template<typename T, typename U, floating_only<T> = 0, floating_only<T> = 0>
    constexpr auto scalar_equal(const Scalar<T> &a, const Scalar<U> &b) { return std::abs(b-a) <= std::numeric_limits<T>::epsilon(); }

    template<typename T, typename U, integral_only<T> = 0, integral_only<U> = 0>
    constexpr auto scalar_equal(const Scalar<T> &a, const Scalar<U> &b) { return (a==b);}

    template<typename T, typename U, integral_only<T> = 0, floating_only<U> = 0>
    constexpr auto scalar_equal(const Scalar<T> &a, const Scalar<U> &b) { return std::abs(b-a) <= std::numeric_limits<T>::epsilon(); }
}}