#pragma once

#include <ray/math/Vector3.hpp>
#include <ostream>

namespace ray { namespace math {

    template<typename T>
    struct Vector4
    {
        using scalar  = Scalar<T>;
        using vector2 = Vector2<T>;
        using vector3 = Vector3<T>;
        using vector4 = Vector4<T>;

        union 
        { 
             struct { scalar x,y,z,w; };
             struct { vector2 xy; vector2 zw; };
             struct { scalar _x; vector2 yz; scalar _w; };
             struct { scalar __x; scalar __y; vector2 _zw; };
             struct { vector3 xyz; scalar __w; };
             struct { scalar ___x; vector3 yzw; };

             struct { scalar r,g,b,a; };  
             struct { vector2 rg; vector2 ba; };
             struct { scalar _r; vector2 gb; scalar _a; };
             struct { scalar __r; scalar __g; vector2 _ba; };
             struct { vector3 rgb; scalar __a; };
             struct { scalar ___r; vector3 gba; };             
        };
        
        constexpr Vector4() = default;
        constexpr Vector4(const vector4 &other) = default;
        constexpr Vector4(scalar v) : x{v}, y{v}, z{v}, w(v) {}
        constexpr Vector4(scalar x, scalar y, scalar z, scalar w) : x(x), y(y), z{z}, w{w} {}
        constexpr Vector4(vector2 xy, scalar z, scalar w) : xy{xy}, zw{z,w} {}
        constexpr Vector4(vector2 xy, vector2 zw) : xy{xy}, zw{zw} {}
        constexpr Vector4(scalar x, vector2 yz, scalar w) : _x{x}, yz{yz}, _w{w} {}
        constexpr Vector4(scalar x, scalar y, vector2 zw) : __x{x}, __y{y}, _zw{zw} {}
        constexpr Vector4(vector3 xyz, scalar w) : xyz{xyz}, __w{w} {}
        constexpr Vector4(scalar x, vector3 yzw) : ___x{x}, yzw{yzw} {}

        constexpr vector4 &operator=(const vector4 &other) = default;

        constexpr auto operator+=(const vector4 &other) { x+=other.x; y+=other.y; z+=other.z; w+=other.w; return (*this); }
        constexpr auto operator-=(const vector4 &other) { x-=other.x; y-=other.y; z-=other.z; w-=other.w; return (*this); }
        constexpr auto operator*=(const vector4 &other) { x*=other.x; y*=other.y; z*=other.z; w*=other.w; return (*this); }
        constexpr auto operator/=(const vector4 &other) { x/=other.x; y/=other.y; z/=other.z; w/=other.w; return (*this); }
        constexpr auto operator*=(const scalar &factor)  { x*=factor; y*=factor; z*=factor; w*=factor; return (*this); }
        constexpr auto operator/=(const scalar &divider) { x/=divider; y/=divider; z/=divider; w/=divider; return (*this); }
        constexpr auto operator-() const { return vector4{-x,-y,-z,-w}; }
    };

    // TODO(cme): test me
    template<typename U, typename T> constexpr auto vector_cast(const Vector4<T> &a) { return Vector4<U>{ static_cast<U>(a.x), static_cast<U>(a.y), static_cast<U>(a.z), static_cast<U>(a.w) }; }

    template<typename T, typename U> constexpr auto operator+(const Vector4<T> &a, const Vector4<U> &b) { return Vector4<widest<T,U>>{a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w}; }
    template<typename T, typename U> constexpr auto operator-(const Vector4<T> &a, const Vector4<U> &b) { return Vector4<widest<T,U>>{a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w}; }
    template<typename T, typename U> constexpr auto operator*(const Vector4<T> &a, const Vector4<U> &b) { return Vector4<widest<T,U>>{a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w}; }
    template<typename T, typename U> constexpr auto operator/(const Vector4<T> &a, const Vector4<U> &b) { return Vector4<widest<T,U>>{a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w}; }

    template<typename T, typename U> constexpr auto operator*(const Vector4<T> &a, const Scalar<U>  &b) { return Vector4<widest<T,U>>{a.x*b,a.y*b,a.z*b,a.w*b}; }
    template<typename T, typename U> constexpr auto operator*(const Scalar<U>  &b, const Vector4<T> &a) { return Vector4<widest<T,U>>{b*a.x,b*a.y,b*a.z,b*a.w}; }
    template<typename T, typename U> constexpr auto operator/(const Vector4<T> &a, const Scalar<U>  &b) { return Vector4<widest<T,U>>{a.x/b,a.y/b,a.z/b,a.w/b}; }
    template<typename T, typename U> constexpr auto operator/(const Scalar<U>  &b, const Vector4<T> &a) { return Vector4<widest<T,U>>{b/a.x,b/a.y,b/a.z,b/a.w}; }

    template<typename T, typename U> constexpr auto operator==(const Vector4<U> &b, const Vector4<T> &a) { return scalar_equal(a.x,b.x) && scalar_equal(a.y,b.y) && scalar_equal(a.z,b.z) && scalar_equal(a.w,b.w);}
    template<typename T, typename U> constexpr auto operator!=(const Vector4<U> &b, const Vector4<T> &a) { return !(a==b); }

    template<typename T> auto &operator<<(std::ostream &out, const Vector4<T> &v) { return (out << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")"); }

    template<typename T, typename U> constexpr auto dot(const Vector4<T> &a, const Vector4<U> &b) { return a.x*b.x+a.y*b.y+a.z*b.z+a.w*b.w; }        
}}