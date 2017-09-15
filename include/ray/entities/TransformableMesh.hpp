#pragma once

#include <ray/components/Transformable.hpp>
#include <ray/entities/Mesh.hpp>

namespace ray { namespace entities {

    class TransformableMesh : public Mesh, public components::Transformable
    {
        static constexpr size_t N_FLOATS_PER_VERTEX = 8;
    public:
        TransformableMesh(const assets::Wavefront &object) : Mesh(object) {}
        TransformableMesh(const std::string &filename) : Mesh(filename) {} 
    };
}}
