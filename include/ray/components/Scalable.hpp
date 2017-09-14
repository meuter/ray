#pragma once

#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace components {

    class Scalable 
    {	
    public:
        constexpr Scalable() : mScale{1,1,1} {}
        constexpr Scalable(float factor) : mScale(factor, factor, factor) {}
        constexpr Scalable(const math::vec3 &factors) : mScale(factors) {}
        constexpr Scalable(float sx, float sy, float sz) : mScale(sx, sy, sz) {}

        const math::vec3 &scale() const { return mScale; }
        math::mat4 scalingMatrix() const { return scaling(mScale); }
        
        void scale(float factor)                    { scale(factor, factor, factor); } 
        void scale(const math::vec3 &factors)       { scale(factors.x, factors.y, factors.z); } 
        void scale(float fx, float fy, float fz)    { mScale.x *= fx; mScale.y *= fy; mScale.z *= fz; } 
        void scaleTo(const math::vec3 &scale)       { mScale = scale; } 
        void scaleTo(float sx, float sy, float sz)  { mScale = math::vec3{sx, sy, sz}; } 

    private:
        math::vec3 mScale;
    };

}}
