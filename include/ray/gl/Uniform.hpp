#pragma once

#include <ray/gl/Texture.hpp>
#include <ray/gl/Type.hpp>
#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace gl {
    
    template<typename T>
    class Uniform 
    {    
    public:
        constexpr Uniform() : mLocation(0) {}
        void operator=(const T &t) const { set(mLocation, t); }
        void set(const T &t) const { set(mLocation, t); }

        auto type() const { return getType<T>(); }

    private:
        static void set(GLint location, const math::f32 &f) { gl(Uniform1f(location, f)); }
        static void set(GLint location, const gl::sampler2D &ts) { gl(Uniform1i(location, ts.value)); }
        static void set(GLint location, const math::vec2 &v) { gl(Uniform2f(location, v.x, v.y)); }
        static void set(GLint location, const math::vec3 &v) { gl(Uniform3f(location, v.x, v.y, v.z)); }
        static void set(GLint location, const math::vec4 &v) { gl(Uniform4f(location, v.x, v.y, v.z, v.w)); }
        static void set(GLint location, const math::mat4 &m) { gl(UniformMatrix4fv(location, 1, true, &m(0,0))); }
        static void set(GLint location, const math::mat3 &m) { gl(UniformMatrix3fv(location, 1, true, &m(0,0))); }
    
        friend class ShaderProgram;        
        GLint mLocation;
    };

}}
