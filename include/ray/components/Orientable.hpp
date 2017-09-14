#pragma once

#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace components {

    class Orientable
    {
    public:
        constexpr Orientable() : mOrientation(0,0,0,1) {}
        constexpr Orientable(const math::vec3 &direction) : mOrientation(direction, 0) {}
        constexpr Orientable(float x, float y, float z) : Orientable(math::vec3(x, y, z)) {}
        
        math::vec3 left()  const { return mOrientation.left(); }
        math::vec3 right() const { return mOrientation.right(); }
        math::vec3 up()    const { return mOrientation.up(); }
        math::vec3 down()  const { return mOrientation.down(); }
        math::vec3 front() const { return mOrientation.front(); }
        math::vec3 back()  const { return mOrientation.back(); }

        void rotate(const math::vec3 &axis, const math::rad &angle) { mOrientation = normalize(math::quat(axis, angle) * mOrientation); }
        
        const math::quat &orientation() const { return mOrientation; }
        math::mat4 rotationMatrix() const { return rotation(orientation()); }        
    private:
        math::quat mOrientation;
    };

}}