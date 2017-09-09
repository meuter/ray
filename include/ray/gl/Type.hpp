#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Print.hpp>

namespace ray { namespace gl {

    namespace details 
    {
        template<typename U> struct OpenGLType { static constexpr GLenum result = 0; };

        template<> struct OpenGLType<GLfloat> { static constexpr GLenum result = GL_FLOAT; }; 
        template<> struct OpenGLType<sampler2D> { static constexpr GLenum result = GL_SAMPLER_2D; }; 
    }

    std::string getTypeName(GLenum type) 
    {
        switch(type) {
            case GL_FLOAT: return "float";
            case GL_FLOAT_VEC2: return "vec2";
            case GL_FLOAT_VEC3: return "vec3";
            case GL_FLOAT_VEC4: return "vec4";
            case GL_FLOAT_MAT4: return "mat4";
            case GL_SAMPLER_2D: return "sampler2D";
            default: return platform::fmt("unknown(%d)", type);
        }
    }

    template<typename T>
    GLenum getType() 
    {
        return details::OpenGLType<T>::result;
    }

}}



