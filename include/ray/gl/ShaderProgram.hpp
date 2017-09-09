#pragma once

#include <ray/gl/Handle.hpp>
#include <ray/platform/Panic.hpp>
#include <ray/gl/Attribute.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/Uniform.hpp>
#include <ray/gl/Type.hpp>
#include <unordered_map>

namespace ray { namespace gl {

    class ShaderProgram 
    {
    public:
        ShaderProgram() = default;

        void use() const { glUseProgram(mHandle); }

        template<GLenum shaderType>
        void attach(const Shader<shaderType> &shader) const
        {
            glAttachShader(mHandle, shader.mHandle); 
        }

        void link()
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
            
            GLint uniformCount = 0;
            glGetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &uniformCount);
    
            for (GLint uniformIndex = 0; uniformIndex < uniformCount; uniformIndex++)
            {
                char uniformName[512];
                GLint uniformSize;
                GLenum uniformType;
    
                glGetActiveUniform(mHandle, (GLuint)uniformIndex, sizeof(uniformName), nullptr, &uniformSize, &uniformType, uniformName);
                auto uniformLocation = glGetUniformLocation(mHandle, uniformName);
    
                mUniformTypesByLocation[uniformLocation] = uniformType;
                mUniformLocationByName[uniformName] = uniformLocation;
            }
            mLinked = true;                        
        }

        template<typename T>
        void bind(Uniform<T> &uniform, const std::string &name) 
        {	
            panicif(!mLinked, "uniforms must be bound after the program has been linked");
            auto hit = mUniformLocationByName.find(name);
            panicif(hit == mUniformLocationByName.end(), "cannot find location of uniform '%s'", name);
            uniform.mLocation = hit->second;           
            
            auto dynamicType = mUniformTypesByLocation.at(uniform.mLocation);
            auto staticType  = getType<T>();
            if (dynamicType != staticType) 
            {
                auto staticName = getTypeName(staticType);
                auto dynamicName = getTypeName(dynamicType);
                panic("type mismatch: uniform '%s' is declared as '%s', but in shader code, it is declared as '%s'", name, staticName, dynamicName);
            }
        }

        void bind(Attribute attribute, const std::string &name)
        {
            panicif(mLinked, "attrbiutes must be bound before the program has been linked");
            glBindAttribLocation(mHandle, attribute.mLocation, name.c_str());
        }

    private:
        static void create(GLuint &handle) { handle = glCreateProgram(); }
        static void destroy(GLuint handle) { glDeleteProgram(handle); }
        Handle<create, destroy> mHandle;
        std::unordered_map<GLint, GLenum> mUniformTypesByLocation;
        std::unordered_map<std::string, GLint> mUniformLocationByName;    
        bool mLinked=false, mUniformsCollected=false;
    };

}}
