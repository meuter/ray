#pragma once

#include <ray/math/Vector3.hpp>
#include <ray/math/Quaternion.hpp>
#include <ray/math/Matrix.hpp>
#include <ray/math/Trigonometry.hpp>

namespace ray { namespace math {

    template<typename S>
    constexpr auto translation(const Scalar<S> &dx, const Scalar<S> &dy, const Scalar<S> &dz)
    {
        return Matrix<S,4,4>{
            1.0f, 0.0f, 0.0f, dx,
            0.0f, 1.0f, 0.0f, dy,
            0.0f, 0.0f, 1.0f, dz,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    template<typename S>
    constexpr auto scaling(const Scalar<S> &sx, const Scalar<S> &sy, const Scalar<S> &sz)
    {
        return Matrix<S,4,4>
        {
            sx,   0.0f, 0.0f, 0.0f,
            0.0f, sy,   0.0f, 0.0f,
            0.0f, 0.0f, sz,   0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    template<typename S>
    constexpr auto rotation(const Vector3<S> &left, const Vector3<S> &up, const Vector3<S> &front)
    {
        return Matrix<S,4,4>{
            left.x,  up.x,  front.x,  0.0f,
            left.y,  up.y,  front.y,  0.0f,
            left.z,  up.z,  front.z,  0.0f,
            0.0f,    0.0f,  0.0f,     1.0f
        };
    }

    template<typename S>
    constexpr auto perspective(rad fovy, const Scalar<S> &aspect, const Scalar<S> &near, const Scalar<S> &far) 
	{
        float rz = far-near;
        float sy = 1/(tan(fovy*0.5f));
        float sx = sy/aspect;
        float sz = -(far+near)/rz;
        float dz = -2*far*near/rz;

        return Matrix<S,4,4>{
            sx,   0.0f,  0.0f, 0.0f,
            0.0f, sy,    0.0f, 0.0f, 
            0.0f, 0.0f,  sz,   dz,
            0.0f, 0.0f, -1.0f, 0.0f
        };
    }

    template<typename S>
    constexpr auto lookAt(const Vector3<S> &eye, const Vector3<S> &center, const Vector3<S> &up)
    {
        const auto f = normalize(center - eye);
        const auto s = normalize(cross(f, up));
        const auto u = cross(s, f);

        return Matrix<S,4,4>{
            s.x, s.y, s.z, -dot(s, eye),
            u.x, u.y, u.z, -dot(u, eye),
            -f.x, -f.y, -f.z, dot(f, eye),
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    template<typename S> constexpr auto translation(const Vector3<S> &displacement) { return translation(displacement.x, displacement.y, displacement.z); }
    template<typename S> constexpr auto scaling(const Vector3<S> &scaleFactor)      { return scaling(scaleFactor.x, scaleFactor.y, scaleFactor.z); }
    template<typename S> constexpr auto scaling(const Scalar<S> &scaleFactor)      { return scaling(scaleFactor, scaleFactor, scaleFactor); }
    template<typename S> constexpr auto rotation(const Quaternion<S> &orientation)  { return rotation(orientation.left(), orientation.up(), orientation.front()); }
    template<typename S> constexpr auto rotation(const Vector3<S> axis, rad angle)  { return rotation(Quaternion<S>(axis, angle)); }
    
}}
