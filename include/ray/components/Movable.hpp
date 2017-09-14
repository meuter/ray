#pragma once

#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace components {

    class Movable
    {
    public:
        constexpr Movable() : mPosition(0,0,0) {}
        constexpr Movable(const math::vec3 &position) : mPosition(position) {}
        constexpr Movable(float x, float y, float z) : mPosition(x, y, z) {}
    
        void move(const math::vec3 &direction, float amount) { mPosition += amount*direction; }
        void moveTo(const math::vec3 &position) { mPosition = position; }
        void moveTo(float x, float y, float z) { mPosition = math::vec3(x,y,z); }
    
        const math::vec3 &position() const { return mPosition; }
        const math::mat4 translationMatrix() const { return translation(mPosition); }
    private:
        math::vec3 mPosition;
    };
    
}}
