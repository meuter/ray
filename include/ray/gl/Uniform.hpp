#pragma once

#include <ray/gl/Texture.hpp>

namespace ray { namespace gl {
    
    template<typename T>
    class Uniform 
    {    
    public:
        constexpr Uniform() : mLocation(0) {}
        inline void operator=(const T &t) const { set(mLocation, t); }

    private:
        static inline void set(GLint location, const GLfloat &f) { glUniform1f(location, f);  }
        static inline void set(GLint location, const gl::sampler2D &ts) { glUniform1i(location, ts.value); }
    
        friend class ShaderProgram;        
        GLint mLocation;
    };

}}
