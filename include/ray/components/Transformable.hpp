#pragma once

#include <ray/components/Movable.hpp>
#include <ray/components/Orientable.hpp>
#include <ray/components/Scalable.hpp>
#include <ray/math/Transform.hpp>

namespace ray { namespace components {

    class Transformable : public Movable, public Orientable, public Scalable
    {	
    public:
        Transformable() : Movable(0,0,0), Orientable(0,0,1), Scalable(1,1,1) {}
        math::mat4 modelMatrix() const { return translationMatrix() * rotationMatrix() * scalingMatrix(); }
    };

}}
