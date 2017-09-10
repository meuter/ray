#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Print.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace gl {

    namespace details 
    {
        template<typename U> struct OpenGLType { static constexpr GLenum result = 0; };

        template<> struct OpenGLType<math::f32>  { static constexpr GLenum result = GL_FLOAT; }; 
        template<> struct OpenGLType<math::u8>  { static constexpr GLenum result = GL_UNSIGNED_BYTE; }; 
        template<> struct OpenGLType<math::vec2> { static constexpr GLenum result = GL_FLOAT_VEC2; }; 
        template<> struct OpenGLType<math::vec3> { static constexpr GLenum result = GL_FLOAT_VEC3; }; 
        template<> struct OpenGLType<math::vec4> { static constexpr GLenum result = GL_FLOAT_VEC4; }; 
        template<> struct OpenGLType<math::mat4> { static constexpr GLenum result = GL_FLOAT_MAT4; }; 
        template<> struct OpenGLType<sampler2D>  { static constexpr GLenum result = GL_SAMPLER_2D; }; 
    }

    std::string getTypeName(GLenum type) 
    {
        switch(type) {
            case GL_FLOAT: return "float";
            case GL_UNSIGNED_BYTE: return "unsigned byte";
            case GL_FLOAT_VEC2: return "vec2";
            case GL_FLOAT_VEC3: return "vec3";
            case GL_FLOAT_VEC4: return "vec4";
            case GL_FLOAT_MAT4: return "mat4";
            case GL_SAMPLER_2D: return "sampler2D";            
            default: return platform::fmt("unknown(%d)", type);
        }
    }

    template<typename T>
    constexpr GLenum getType() 
    {
        return details::OpenGLType<T>::result;
    }

}}



