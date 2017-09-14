#pragma once

#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace assets {

    using PackedColor = math::Vector4<math::u8>;
    using UnpackedColor = math::Vector4<math::f32>;
    
    static constexpr auto pack(const UnpackedColor &unpacked) { return math::vector_cast<math::u8>(unpacked*255);}
    static constexpr auto unpack(const PackedColor &packed)   { return math::vector_cast<math::f32>(packed/255.0f); }

}}