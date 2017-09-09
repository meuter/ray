#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Panic.hpp>
#include <ray/math/LinearAlgebra.hpp>

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

    private:
        friend class ShaderProgram;
        GLint mLocation = 0;
    };

    static constexpr auto ATTRIBUTE_POSITION2D = Attribute<math::vec2>{0};
    static constexpr auto ATTRIBUTE_POSITION3D = Attribute<math::vec3>{1};
    static constexpr auto ATTRIBUTE_TEXCOORD   = Attribute<math::vec2>{2};
    static constexpr auto ATTRIBUTE_NORMAL     = Attribute<math::vec3>{3};
    static constexpr auto ATTRIBUTE_COLOR4F    = Attribute<math::vec4>{4};
    static constexpr auto ATTRIBUTE_COLORU32   = Attribute<math::u32>{4};
    
}}