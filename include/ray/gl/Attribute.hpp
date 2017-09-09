#pragma once

#include <ray/glfw/glad.hpp>

namespace ray { namespace gl {

    struct Attribute 
    {
        constexpr Attribute() = default;
        constexpr Attribute(GLint location) : mLocation(location) {}

        void enable() const { glEnableVertexAttribArray(mLocation); }
        void disable() const { glDisableVertexAttribArray(mLocation); }

        void pointer(GLint count, GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
        {
            glVertexAttribPointer(mLocation, count, type, normalized, stride, (GLvoid *)(offset));
        }

        constexpr auto location() const { return mLocation; }

    private:
        GLint mLocation = 0;
    };

    static constexpr auto ATTRIBUTE_POSITION = Attribute{0};
    static constexpr auto ATTRIBUTE_TEXCOORD = Attribute{1};
    static constexpr auto ATTRIBUTE_NORMAL   = Attribute{2};
    static constexpr auto ATTRIBUTE_COLOR    = Attribute{3};
    static constexpr auto ATTRIBUTE_FONTSIZE = Attribute{4};
    
}}