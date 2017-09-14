#include <ray/math/Scalar.hpp>
#include <ray/math/Vector2.hpp>
#include <ray/math/Vector3.hpp>
#include <ray/math/Vector4.hpp>
#include <ray/math/Matrix.hpp>
#include <ray/math/Quaternion.hpp>
#include <ray/math/Trigonometry.hpp>
#include <ray/math/Utils.hpp>
#include <ray/math/Transform.hpp>
#include <ray/math/Rectangle.hpp>

namespace ray { namespace math {

    using u8 = uint8_t;
    using i8 = int8_t;

    using u16 = uint16_t;
    using i16 = int16_t;

    using i32 = int32_t;
    using u32 = uint32_t;
    using f32 = float;

    using i64 = int64_t;
    using u64 = uint64_t;
    using f64 = double;

    using vec2  = Vector2<f32>;
    using dvec2 = Vector2<f64>;
    using ivec2 = Vector2<i32>;
    using uvec2 = Vector2<u32>;

    using vec3  = Vector3<f32>;
    using dvec3 = Vector3<f64>;
    using ivec3 = Vector3<i32>;
    using uvec3 = Vector3<u32>;

    using vec4  = Vector4<f32>;
    using dvec4 = Vector4<f64>;
    using ivec4 = Vector4<i32>;
    using uvec4 = Vector4<u32>;

    using rect2  = Rectangle<Vector2,f32>;
    using irect2 = Rectangle<Vector2,i32>;
    using urect2 = Rectangle<Vector2,u32>;
    using drect2 = Rectangle<Vector2,f64>;

    using rect3  = Rectangle<Vector3,f32>;
    using irect3 = Rectangle<Vector3,i32>;
    using urect3 = Rectangle<Vector3,u32>;
    using drect3 = Rectangle<Vector3,f64>;

    using rect4  = Rectangle<Vector4,f32>;
    using irect4 = Rectangle<Vector4,i32>;
    using urect4 = Rectangle<Vector4,u32>;
    using drect4 = Rectangle<Vector4,f64>;
    
    using mat2  = Matrix<f32, 2, 2>;
    using imat2 = Matrix<i32, 2, 2>;
    using umat2 = Matrix<u32, 2, 2>;
    using dmat2 = Matrix<f64, 2, 2>;

    using mat3  = Matrix<f32, 3, 3>;
    using imat3 = Matrix<i32, 3, 3>;
    using umat3 = Matrix<u32, 3, 3>;
    using dmat3 = Matrix<f64, 3, 3>;
    
    using mat4  = Matrix<f32, 4, 4>;
    using imat4 = Matrix<i32, 4, 4>;
    using umat4 = Matrix<u32, 4, 4>;
    using dmat4 = Matrix<f64, 4, 4>;

    using quat  = Quaternion<f32>;
    using dquat = Quaternion<f64>;
}}