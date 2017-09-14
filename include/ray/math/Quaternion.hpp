#pragma once

#include <ray/math/Vector3.hpp>
#include <ray/math/Vector4.hpp>
#include <ray/math/Matrix.hpp>
#include <ray/math/Trigonometry.hpp>
#include <ray/math/Utils.hpp>

namespace ray { namespace math {

	template<typename S>
	class Quaternion : public Vector4<S>
	{
        using vec4 = Vector4<S>;
		using vec3 = Vector3<S>;
        using quat = Quaternion<S>;
        using mat4 = Matrix<S,4,4>;

    public:
		using Vector4<S>::x;
		using Vector4<S>::y;
		using Vector4<S>::z;
		using Vector4<S>::w;

        constexpr Quaternion() = default;
        constexpr Quaternion(const vec4 &other) : vec4(other) {}
        constexpr Quaternion(const S &x, const S &y, const S &z, const S &w) : vec4{x,y,z,w} {}
        constexpr Quaternion(const quat &other) = default;
		constexpr Quaternion(const vec3 &axis, const rad &angle) : vec4{axis.x, axis.y, axis.z, 0}
		{		
            auto s = (S)sin(angle/2);
            auto c = (S)cos(angle/2);
            (*this)= normalize(vec4(axis * s, c));
        }

        constexpr auto operator*=(const quat &r) 
        { 
            auto nx = w*r.x + x*r.w + y*r.z - z*r.y;
            auto ny = w*r.y + y*r.w + z*r.x - x*r.z;
            auto nz = w*r.z + z*r.w + x*r.y - y*r.x;
            auto nw = w*r.w - x*r.x - y*r.y - z*r.z;
            this->x = nx; this->y = ny; this->z = nz; this->w = nw;
            return (*this);
        }

        vec3 left()  const { return  vec3(1,0,0)*(*this); }
        vec3 right() const { return -vec3(1,0,0)*(*this); }
        vec3 up()    const { return  vec3(0,1,0)*(*this); }
        vec3 down()  const { return -vec3(0,1,0)*(*this); }
        vec3 front() const { return  vec3(0,0,1)*(*this); }
        vec3 back()  const { return -vec3(0,0,1)*(*this); }

        mat4 toMatrix() const
        {
            auto l = left();
            auto u = up();
            auto f = front();
    
            return mat4{
                l.x,  u.x,  f.x,  0.0f,
                l.y,  u.y,  f.y,  0.0f,
                l.z,  u.z,  f.z,  0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };
        }
    };
    
    template<typename S> constexpr auto conjugate(const Quaternion<S> &q) { return Quaternion<S>{-q.x,-q.y,-q.z,q.w}; }

    template<typename S, typename T> constexpr auto operator*(const Quaternion<S> &l, const Quaternion<T> &r) 
    { 
        Quaternion<widest<S,T>> result{l}; return (result*=r); 
    }

    template<typename S, typename T> constexpr auto operator*(const Vector3<S> &v, const Quaternion<T> &q) 
    {
        return (q * Quaternion<S>(v.x, v.y, v.z, 0) * conjugate(q)).xyz;
    }

}}

