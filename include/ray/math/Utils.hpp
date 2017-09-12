#pragma once

#include <cmath>

namespace ray { namespace math {

    template<typename T> constexpr auto length(const T &v) { return std::sqrt(math::dot(v,v)); }
    template<typename T> constexpr auto normalize(const T &v) { return v/math::length(v); }

    template<typename T> constexpr auto clamp(const T &lower, const T &value, const T &upper) { return std::max(lower, std::min(upper, value)); }
    template<typename T> constexpr auto clamp01(const T &value) { return clamp(T{0}, value, T{1}); }

    template<typename T> constexpr auto lerp(const T &start, const T &end, const double &percent) { return start + percent * (end-start); }
    template<typename T> constexpr auto nlerp(const T &start, const T &end, const double &percent) { return normalize(lerp(start, end, percent)); }

    template<typename T>
    constexpr auto slerp(const T &start, const T &end, const double &percent)
    {
         auto cosTheta = dot(start, end);
         auto theta = acos(cosTheta) * percent;
         auto relative = normalize(((end - start) * cosTheta));

         return start * std::cos(theta) + relative * std::sin(theta);
    }
    
}}

