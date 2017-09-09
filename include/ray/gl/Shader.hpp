#pragma once

#include <ray/gl/Handle.hpp>
#include <ray/platform/Panic.hpp>

#define GLSL(version, x) "#version " #version "\n" #x

namespace ray { namespace gl {

    template<GLenum shaderType>
    class Shader
    {
    public:

        Shader() = default;
        Shader(const std::string &shaderText) { loadSource(shaderText); }

        void loadSource(const std::string &shaderText)
        {
            GLint success;
            const GLchar *sources[] = { shaderText.c_str() };
    
            gl(ShaderSource(mHandle, 1, sources, NULL));
            gl(CompileShader(mHandle));
            gl(GetShaderiv(mHandle, GL_COMPILE_STATUS, &success));
    
            if (!success)
            {
                char errorMessage[1024];
                gl(GetShaderInfoLog(mHandle, sizeof(errorMessage), NULL, errorMessage));
                panic(" could not compile %s shader\n%s", (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment", errorMessage);
            }
        }

    private:
        friend class ShaderProgram;
        
        static void create(GLuint &handle) { handle = glCreateShader(shaderType); }
        static void destroy(GLuint handle) { gl(DeleteShader(handle)); }
        Handle<create, destroy> mHandle;
    };

    using FragmentShader = Shader<GL_FRAGMENT_SHADER>;
    using VertexShader   = Shader<GL_VERTEX_SHADER>;
    using GeometryShader = Shader<GL_GEOMETRY_SHADER>;

}}