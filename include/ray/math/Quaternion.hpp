#pragma once

#include <ray/math/Vector3.hpp>
#include <ray/math/Vector4.hpp>
#include <ray/math/Trigonometry.hpp>
#include <ray/math/Utils.hpp>

namespace ray { namespace math {

	template<typename S>
	class Quaternion : public Vector4<S>
	{
        using vec4 = Vector4<S>;
		using vec3 = Vector3<S>;
		using quat = Quaternion<S>;

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
    };
    
    template<typename S> constexpr auto conjugate(const Quaternion<S> &q) { return Quaternion<S>{-q.x,-q.y,-q.z,q.w}; }

    template<typename S, typename T> constexpr auto operator*(const Quaternion<S> &l, const Quaternion<T> &r) 
    {
        return Quaternion<widest<S,T>>{
            l.w * r.x + l.x * r.w + l.y * r.z - l.z * r.y,
            l.w * r.y + l.y * r.w + l.z * r.x - l.x * r.z,
            l.w * r.z + l.z * r.w + l.x * r.y - l.y * r.x,
            l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z
        };
    }

    template<typename S, typename T> constexpr auto rotate(const Vector3<S> &v, const Quaternion<T> &q) 
    {
        return (q * Quaternion<S>(v.x, v.y, v.z, 0) * conjugate(q)).xyz;
    }

    template<typename S, typename T> constexpr auto rotate(const Vector3<S> &v, const Vector3<T> &axis, const rad &angle)
    {
        return rotate(v, Quaternion<T>(axis, angle));
    }
}}

