#pragma once

#include <ray/math/Scalar.hpp>
#include <ostream>

namespace ray { namespace math {

    template<typename T>
    struct Vector2
    {
        using scalar = Scalar<T>;
        using vector2 = Vector2<T>;

        union {   
            struct { scalar x,y; };
            struct { scalar u,v; };  
            struct { scalar w,h; };  
        };

        constexpr Vector2() = default;
        constexpr Vector2(const vector2 &other) = default;
        constexpr Vector2(scalar v) : x{v}, y{v} {}
        constexpr Vector2(scalar x, scalar y) : x(x), y(y) {}

        constexpr vector2 &operator=(const vector2 &other) = default;

        constexpr auto operator+=(const vector2 &other) { x+=other.x; y+=other.y; return (*this); }
        constexpr auto operator-=(const vector2 &other) { x-=other.x; y-=other.y; return (*this); }
        constexpr auto operator*=(const vector2 &other) { x*=other.x; y*=other.y; return (*this); }
        constexpr auto operator/=(const vector2 &other) { x/=other.x; y/=other.y; return (*this); }
        constexpr auto operator*=(const scalar &other) { x*=other; y*=other; return (*this); }
        constexpr auto operator/=(const scalar &other) { x/=other; y/=other; return (*this); }
        constexpr auto operator-() const { return vector2{-x,-y}; }
    };

    // TODO(cme): test me
    template<typename U, typename T> constexpr auto vector_cast(const Vector2<T> &a) { return Vector2<U>{ static_cast<U>(a.x), static_cast<U>(a.y) }; }
        
    template<typename T, typename U> constexpr auto operator+(const Vector2<T> &a, const Vector2<U> &b) { return Vector2<widest<T,U>>{a.x+b.x, a.y+b.y}; }
    template<typename T, typename U> constexpr auto operator-(const Vector2<T> &a, const Vector2<U> &b) { return Vector2<widest<T,U>>{a.x-b.x, a.y-b.y}; }
    template<typename T, typename U> constexpr auto operator*(const Vector2<T> &a, const Vector2<U> &b) { return Vector2<widest<T,U>>{a.x*b.x, a.y*b.y}; }
    template<typename T, typename U> constexpr auto operator/(const Vector2<T> &a, const Vector2<U> &b) { return Vector2<widest<T,U>>{a.x/b.x, a.y/b.y}; }

    template<typename T, typename U> constexpr auto operator*(const Vector2<T> &a, const Scalar<U>  &b) { return Vector2<widest<T,U>>{a.x*b,a.y*b}; }
    template<typename T, typename U> constexpr auto operator*(const Scalar<U>  &b, const Vector2<T> &a) { return Vector2<widest<T,U>>{b*a.x,b*a.y}; }
    template<typename T, typename U> constexpr auto operator/(const Vector2<T> &a, const Scalar<U>  &b) { return Vector2<widest<T,U>>{a.x/b,a.y/b}; }
    template<typename T, typename U> constexpr auto operator/(const Scalar<U>  &b, const Vector2<T> &a) { return Vector2<widest<T,U>>{b/a.x, b/a.y}; }

    template<typename T, typename U> constexpr auto operator==(const Vector2<U> &b, const Vector2<T> &a) { return scalar_equal(a.x,b.x) && scalar_equal(a.y,b.y); }
    template<typename T, typename U> constexpr auto operator!=(const Vector2<U> &b, const Vector2<T> &a) { return !(a==b); }
    
    template<typename T> auto &operator<<(std::ostream &out, const Vector2<T> &v) { return (out << "(" << v.x << "," << v.y << ")"); }

    template<typename T> constexpr auto dot(const Vector2<T> &a, const Vector2<T> &b) { return a.x*b.x + a.y*b.y; }
}}