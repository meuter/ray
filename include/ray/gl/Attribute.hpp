#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Panic.hpp>
#include <ray/math/LinearAlgebra.hpp>
#include <ray/gl/Type.hpp>

namespace ray { namespace gl {

    template<typename T>
    struct Attribute 
    {
        constexpr Attribute() = default;
        constexpr Attribute(GLint location) : mLocation(location) {}

        void enable() const { gl(EnableVertexAttribArray(mLocation)); }
        void disable() const { gl(DisableVertexAttribArray(mLocation)); }

        void pointer(GLint count, GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
        {
            gl(VertexAttribPointer(mLocation, count, type, normalized, stride, (GLvoid *)(offset)));
        }

        auto size() const { return sizeof(T); }
        auto type() const { return getType<T>(); }

    private:
        friend class ShaderProgram;
        GLint mLocation = 0;
    };

    static constexpr auto ATTRIBUTE_2F_POSITION  = Attribute<math::vec2>{0};
    static constexpr auto ATTRIBUTE_3F_POSITION  = Attribute<math::vec3>{1};
    static constexpr auto ATTRIBUTE_2F_TEXCOORD  = Attribute<math::vec2>{2};
    static constexpr auto ATTRIBUTE_3F_NORMAL    = Attribute<math::vec3>{3};
    static constexpr auto ATTRIBUTE_4F_COLOR     = Attribute<math::vec4>{4};
    static constexpr auto ATTRIBUTE_1I_COLORU_1I = Attribute<math::u32>{4};
    
}}