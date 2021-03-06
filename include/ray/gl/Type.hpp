#pragma once

#include <ray/platform/OpenGL.hpp>
#include <ray/platform/Print.hpp>
#include <ray/gl/AbstractTexture.hpp>
#include <ray/math/LinearAlgebra.hpp>
#include <ray/assets/Color.hpp>

namespace ray { namespace gl {

    namespace details 
    {
        template<typename U> struct OpenGLType      { };
        template<> struct OpenGLType<math::f32>     { static constexpr GLenum value = GL_FLOAT; }; 
        template<> struct OpenGLType<math::u8>      { static constexpr GLenum value = GL_UNSIGNED_BYTE; }; 
        template<> struct OpenGLType<math::vec2>    { static constexpr GLenum value = GL_FLOAT_VEC2; }; 
        template<> struct OpenGLType<math::vec3>    { static constexpr GLenum value = GL_FLOAT_VEC3; }; 
        template<> struct OpenGLType<math::vec4>    { static constexpr GLenum value = GL_FLOAT_VEC4; }; 
        template<> struct OpenGLType<math::mat3>    { static constexpr GLenum value = GL_FLOAT_MAT3; }; 
        template<> struct OpenGLType<math::mat4>    { static constexpr GLenum value = GL_FLOAT_MAT4; };         
        template<> struct OpenGLType<assets::Color> { static constexpr GLenum value = GL_UNSIGNED_BYTE; }; 
        template<> struct OpenGLType<sampler<GL_TEXTURE_CUBE_MAP>> { static constexpr GLenum value = GL_SAMPLER_CUBE; }; 
        template<> struct OpenGLType<sampler<GL_TEXTURE_2D>>       { static constexpr GLenum value = GL_SAMPLER_2D; }; 

    }

    std::string getTypeName(GLenum type) 
    {
        switch(type) {
            case GL_FLOAT: return "float";
            case GL_UNSIGNED_BYTE: return "unsigned byte";
            case GL_FLOAT_VEC2: return "vec2";
            case GL_FLOAT_VEC3: return "vec3";
            case GL_FLOAT_VEC4: return "vec4";
            case GL_FLOAT_MAT3: return "mat3";
            case GL_FLOAT_MAT4: return "mat4";
            case GL_SAMPLER_2D: return "sampler2D";   
            case GL_SAMPLER_CUBE: return "samplerCube";        
            default: return platform::fmt("unknown(%d)", type);
        }
    }

    template<typename T>
    constexpr GLenum getType() 
    {
        return details::OpenGLType<T>::value;
    }    

}}



