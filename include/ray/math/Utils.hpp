#pragma once

#include <cmath>
#include <ray/math/Vector2.hpp>
#include <ray/math/Vector3.hpp>
#include <ray/math/Vector4.hpp>

namespace ray { namespace math {

    template<template<typename U> class Vector, typename T> constexpr auto length(const Vector<T> &v) { return std::sqrt(dot(v,v)); }
    template<template<typename U> class Vector, typename T> constexpr auto normalize(const Vector<T> &v) { return v/length(v); }

    template<typename T> constexpr auto clamp(const T &lower, const T &value, const T &upper) { return std::max(lower, std::min(upper, value)); }
    template<typename T> constexpr auto clamp01(const T &value) { return clamp(T{0}, value, T{1}); }

    template<typename T> constexpr auto lerp(const T &start, const T &end, const double &t) { return (1-t)*start + t*end; }
    template<typename T> constexpr auto nlerp(const T &start, const T &end, const double &t) { return normalize(lerp(start, end, t)); }    
}}

