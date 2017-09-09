#pragma once

#include <ray/gl/Texture.hpp>
#include <ray/math/LinearAlgebra.hpp>


namespace ray { namespace gl {
    
    template<typename T>
    class Uniform 
    {    
    public:
        constexpr Uniform() : mLocation(0) {}
        void operator=(const T &t) const { set(mLocation, t); }
        void set(const T &t) const { set(mLocation, t); }

    private:
        static void set(GLint location, const math::f32 &f) { glUniform1f(location, f);  }
        static void set(GLint location, const gl::sampler2D &ts) { glUniform1i(location, ts.value); }
        static void set(GLint location, const math::vec2 &v) { glUniform2f(location, v.x, v.y); }
        static void set(GLint location, const math::vec3 &v) { glUniform3f(location, v.x, v.y, v.z); }
        static void set(GLint location, const math::vec4 &v) { glUniform4f(location, v.x, v.y, v.z, v.w); }
        static void set(GLint location, const math::mat4 &m) { glUniformMatrix4fv(location, 1, true, &m(0,0)); }
        static void set(GLint location, const math::mat3 &m) { glUniformMatrix3fv(location, 1, true, &m(0,0)); }
    
        friend class ShaderProgram;        
        GLint mLocation;
    };

}}
