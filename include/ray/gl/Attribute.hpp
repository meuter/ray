#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Panic.hpp>
#include <ray/gl/Type.hpp>

namespace ray { namespace gl {

    template<typename V, typename F>
    struct Attribute 
    {
        constexpr Attribute() = default;
        constexpr Attribute(GLint location) : mLocation(location) {}

        void enable() const { gl(EnableVertexAttribArray(mLocation)); }
        void disable() const { gl(DisableVertexAttribArray(mLocation)); }

        void pointer(GLboolean normalized, GLsizei stride, size_t offset)
        {
            gl(VertexAttribPointer(mLocation, scalarCount(), scalarType(), normalized, stride*scalarSize(), (GLvoid *)(offset*scalarSize())));
        }

        constexpr auto type() const { return getType<V>(); }
        constexpr auto size() const { return sizeof(V); }
        constexpr auto scalarType() const { return getType<F>(); }
        constexpr auto scalarSize() const { return sizeof(F); }
        constexpr auto scalarCount() const { return size()/scalarSize(); }

    private:
        friend class ShaderProgram;
        GLint mLocation = 0;
    };

    static constexpr auto ATTRIBUTE_2F_POSITION  = Attribute<math::vec2,math::f32>{0};
    static constexpr auto ATTRIBUTE_3F_POSITION  = Attribute<math::vec3,math::f32>{1};
    static constexpr auto ATTRIBUTE_2F_TEXCOORD  = Attribute<math::vec2,math::f32>{2};
    static constexpr auto ATTRIBUTE_3F_NORMAL    = Attribute<math::vec3,math::f32>{3};
    static constexpr auto ATTRIBUTE_4F_COLOR     = Attribute<math::vec4,math::f32>{4};
    static constexpr auto ATTRIBUTE_1I_COLORU_1I = Attribute<math::u32,math::u32>{5};
    
}}