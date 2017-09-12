#pragma once

#include <ray/math/Vector2.hpp>
#include <ostream>

namespace ray { namespace math {

    template<typename T>
    struct Vector3
    {
        using scalar  = Scalar<T>;
        using vector2 = Vector2<T>;
        using vector3 = Vector3<T>;

        union 
        { 
            struct { scalar x,y,z; };
            struct { vector2 xy; scalar _z; };
            struct { scalar _x; vector2 yz; };

            struct { scalar r,g,b; };  
            struct { vector2 rg; scalar _b; };
            struct { scalar _r; vector2 gb; };

            struct { scalar w,h,d; };  
            struct { vector2 wh; scalar _d; };
            struct { scalar _w; vector2 hd; };
        };

        constexpr Vector3() = default;
        constexpr Vector3(const vector3 &other) = default;
        constexpr Vector3(scalar v) : x{v}, y{v}, z{v} {}
        constexpr Vector3(scalar x, scalar y, scalar z) : x(x), y(y), z{z} {}
        constexpr Vector3(vector2 xy, scalar z) : xy{xy}, _z{z} {}
        constexpr Vector3(scalar x, vector2 yz) : _x{x}, yz{yz} {}

        constexpr vector3 &operator=(const vector3 &other) = default;
        
        constexpr auto operator+=(const vector3 &other) { x+=other.x; y+=other.y; z+=other.z; return (*this); }
        constexpr auto operator-=(const vector3 &other) { x-=other.x; y-=other.y; z-=other.z; return (*this); }
        constexpr auto operator*=(const vector3 &other) { x*=other.x; y*=other.y; z*=other.z; return (*this); }
        constexpr auto operator/=(const vector3 &other) { x/=other.x; y/=other.y; z/=other.z; return (*this); }
        constexpr auto operator*=(const scalar &factor) { x*=factor; y*=factor; z*=factor; return (*this); }
        constexpr auto operator/=(const scalar &divider) { x/=divider; y/=divider; z/=divider; return (*this); }
        constexpr auto operator-() const { return vector3{-x,-y,-z}; }
    };

    template<typename U, typename T> constexpr auto vector_cast(const Vector3<T> &a) { return Vector3<U>{ static_cast<U>(a.x), static_cast<U>(a.y), static_cast<U>(a.z) }; }
    
    template<typename T, typename U> constexpr auto operator+(const Vector3<T> &a, const Vector3<U> &b) { return Vector3<widest<T,U>>{a.x+b.x, a.y+b.y, a.z+b.z}; }
    template<typename T, typename U> constexpr auto operator-(const Vector3<T> &a, const Vector3<U> &b) { return Vector3<widest<T,U>>{a.x-b.x, a.y-b.y, a.z-b.z}; }
    template<typename T, typename U> constexpr auto operator*(const Vector3<T> &a, const Vector3<U> &b) { return Vector3<widest<T,U>>{a.x*b.x, a.y*b.y, a.z*b.z}; }
    template<typename T, typename U> constexpr auto operator/(const Vector3<T> &a, const Vector3<U> &b) { return Vector3<widest<T,U>>{a.x/b.x, a.y/b.y, a.z/b.z}; }

    template<typename T, typename U> constexpr auto operator*(const Vector3<T> &a, const Scalar<U>  &b) { return Vector3<widest<T,U>>{a.x*b,a.y*b,a.z*b}; }
    template<typename T, typename U> constexpr auto operator*(const Scalar<U>  &b, const Vector3<T> &a) { return Vector3<widest<T,U>>{b*a.x,b*a.y,b*a.z}; }
    template<typename T, typename U> constexpr auto operator/(const Vector3<T> &a, const Scalar<U>  &b) { return Vector3<widest<T,U>>{a.x/b,a.y/b,a.z/b}; }
    template<typename T, typename U> constexpr auto operator/(const Scalar<U>  &b, const Vector3<T> &a) { return Vector3<widest<T,U>>{b/a.x, b/a.y,b/a.z}; }

    template<typename T, typename U> constexpr auto operator==(const Vector3<U> &b, const Vector3<T> &a) { return scalar_equal(a.x,b.x) && scalar_equal(a.y,b.y) && scalar_equal(a.z,b.z); }
    template<typename T, typename U> constexpr auto operator!=(const Vector3<U> &b, const Vector3<T> &a) { return !(a==b); }

    template<typename T> auto &operator<<(std::ostream &out, const Vector3<T> &v) { return (out << "(" << v.x << "," << v.y << "," << v.z << ")"); }

    template<typename T, typename U> constexpr auto dot(const Vector3<T> &a, const Vector3<U> &b) { return widest<T,U>(a.x*b.x + a.y*b.y + a.z*b.z); }
    template<typename T, typename U> constexpr auto cross(const Vector3<T> &l, const Vector3<U> &r) { return Vector3<widest<T,U>>{ (l.y*r.z)-(r.y*l.z), (l.z*r.x)-(r.z*l.x), (l.x*r.y)-(r.x*l.y) }; }
}}