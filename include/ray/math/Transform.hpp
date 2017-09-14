#pragma once

#include <ray/math/Vector3.hpp>
#include <ray/math/Quaternion.hpp>
#include <ray/math/Matrix.hpp>
#include <ray/math/Trigonometry.hpp>

namespace ray { namespace math {

    template<typename S>
    constexpr auto translation(const Vector3<S> &displacement)
    {
        return Matrix<S,4,4>{
            1.0f, 0.0f, 0.0f, displacement.x,
            0.0f, 1.0f, 0.0f, displacement.y,
            0.0f, 0.0f, 1.0f, displacement.z,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
    }

    template<typename S>
    constexpr auto scaling(const Vector3<S> &scaleFactor)
    {
        return Matrix<S,4,4>
        {
            scaleFactor.x, 0.0f,          0.0f,          0.0f,
            0.0f,          scaleFactor.y, 0.0f,          0.0f,
            0.0f,          0.0f,          scaleFactor.z, 0.0f,
            0.0f,          0.0f,          0.0f,          1.0f
        };
    }

    template<typename S>
    constexpr auto rotation(const Quaternion<S> orientation)
    {
        auto l = orientation.left();
        auto u = orientation.up();
        auto f = orientation.front();

        return Matrix<S,4,4>{
            l.x,  u.x,  f.x,  0.0f,
            l.y,  u.y,  f.y,  0.0f,
            l.z,  u.z,  f.z,  0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    template<typename S>
    constexpr auto projection(rad fovy, Scalar<S> aspect, Scalar<S> near, Scalar<S> far) 
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


}}
