#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Panic.hpp>
#include <ray/gl/Type.hpp>

namespace ray { namespace gl {

    namespace details
    {
        template<typename T> struct ScalarType { };
        template<> struct ScalarType<math::f32>  { using type = math::f32; }; 
        template<> struct ScalarType<math::u8>   { using type = math::u8; }; 
        template<> struct ScalarType<math::vec2> { using type = math::f32; }; 
        template<> struct ScalarType<math::vec3> { using type = math::f32; }; 
        template<> struct ScalarType<math::vec4> { using type = math::f32; }; 
        template<> struct ScalarType<math::mat4> { using type = math::f32; }; 
        template<> struct ScalarType<sampler2D>  { using type = math::u32; }; 
    }

    template<typename V>
    struct Attribute 
    {
    private:
        using F = typename details::ScalarType<V>::type;

    public:
        constexpr Attribute() = default;
        constexpr Attribute(GLint location) : mLocation(location) {}

        template<typename T, size_t stride>
        void bind(const VertexBuffer<T, stride> &vbo, bool normalized, size_t offset)
        {
            vbo.bind();
            gl(EnableVertexAttribArray(mLocation));
            glVertexAttribPointer(mLocation, scalarCount(), getType<T>(), normalized ? GL_TRUE : GL_FALSE, stride * sizeof(T), (GLvoid *)(offset*sizeof(T)));
        }

        constexpr auto type() const { return getType<V>(); }
        constexpr auto size() const { return sizeof(V); }
        constexpr auto scalarType() const { return getType<F>(); }
        constexpr auto scalarSize() const { return sizeof(F); }
        constexpr auto scalarCount() const { return size()/scalarSize(); }

    private:
        GLint mLocation = 0;
    };
    
}}