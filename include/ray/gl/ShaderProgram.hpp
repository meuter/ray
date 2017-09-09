#pragma once

#include <ray/gl/Handle.hpp>
#include <ray/platform/Panic.hpp>
#include <ray/gl/Attribute.hpp>
#include <ray/gl/Shader.hpp>

namespace ray { namespace gl {

    class ShaderProgram 
    {
    public:
        ShaderProgram() = default;

        void use() const { glUseProgram(mHandle); }

        template<GLenum shaderType>
        void attach(const Shader<shaderType> &shader)
        {
            glAttachShader(mHandle, shader.mHandle);
        }

        void link() const
        {
            GLint success;
            
            glLinkProgram(mHandle);
            glGetProgramiv(mHandle, GL_LINK_STATUS, &success);
    
            if (!success)
            {        
                char errorMessage[1024];
                glGetProgramInfoLog(mHandle, sizeof(errorMessage), NULL, errorMessage);
                panic("could not link shader program: %s", errorMessage);
            }
        }

        void bind(Attribute attribute, const std::string &name)
        {
            glBindAttribLocation(mHandle, attribute.location(), name.c_str());
        }

    private:
        static void create(GLuint &handle) { handle = glCreateProgram(); }
        static void destroy(GLuint handle) { glDeleteProgram(handle); }
        Handle<create, destroy> mHandle;
    };

}}
