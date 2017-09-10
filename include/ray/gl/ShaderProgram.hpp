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
        ShaderProgram(const char *vertexShader, const char *fragmentShader)
        {
            attach(VertexShader(vertexShader));
            attach(FragmentShader(fragmentShader));
            link();
            use();
        }

        void use() const { gl(UseProgram(mHandle)); }

        template<GLenum shaderType>
        void attach(const Shader<shaderType> &shader) const
        {
            gl(AttachShader(mHandle, shader.mHandle)); 
        }

        void link()
        {
            GLint success;
            
            gl(LinkProgram(mHandle));
            gl(GetProgramiv(mHandle, GL_LINK_STATUS, &success));
    
            if (!success)
            {        
                char errorMessage[1024];
                gl(GetProgramInfoLog(mHandle, sizeof(errorMessage), NULL, errorMessage));
                panic("could not link shader program: %s", errorMessage);
            }
            
            GLint uniformCount = 0;
            gl(GetProgramiv(mHandle, GL_ACTIVE_UNIFORMS, &uniformCount));

            for (GLint uniformIndex = 0; uniformIndex < uniformCount; uniformIndex++)
            {
                char uniformName[512];
                GLint uniformSize;
                GLenum uniformType;
    
                gl(GetActiveUniform(mHandle, (GLuint)uniformIndex, sizeof(uniformName), nullptr, &uniformSize, &uniformType, uniformName));
                auto uniformLocation = glGetUniformLocation(mHandle, uniformName);
    
                mUniformTypesByLocation[uniformLocation] = uniformType;
                mUniformLocationByName[uniformName] = uniformLocation;
            }

            GLint attributeCount = 0;
            gl(GetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &attributeCount));
    
            for (GLint attributeIndex = 0; attributeIndex < attributeCount; attributeIndex++)
            {
                char attributeName[512];
                GLint attributeSize;
                GLenum attributeType;
    
                gl(GetActiveAttrib(mHandle, (GLuint)attributeIndex, sizeof(attributeName), nullptr, &attributeSize, &attributeType, attributeName));
                auto attributeLocation = glGetAttribLocation(mHandle, attributeName);

                mAttributeTypesByLocation[attributeLocation] = attributeType;
                mAttributeLocationByName[attributeName] = attributeLocation;
            }

            mLinked = true;                        
        }

        template<typename T>
        auto getUniform(const std::string &name)
        {
            panicif(!mLinked, "uniforms must be bound after the program has been linked");
            auto hit = mUniformLocationByName.find(name);
            panicif(hit == mUniformLocationByName.end(), "cannot find location of uniform '%s'", name);
            
            auto typeInShaderCode = mUniformTypesByLocation.at(hit->second);
            auto typeInCPPCode    = getType<T>();
            if (typeInShaderCode != typeInCPPCode) 
            {
                auto cppName = getTypeName(typeInCPPCode);
                auto shaderName = getTypeName(typeInShaderCode);
                panic("type mismatch: uniform '%s' is declared as '%s', but in shader code, it is declared as '%s'", name, cppName, shaderName);
            }

            return Uniform<T>{hit->second};
        }

        template<typename V>
        auto getAttribute(const std::string &name)
        {            
            panicif(!mLinked, "program should be linked before attempting to get an attribute");
            auto hit = mAttributeLocationByName.find(name);
            panicif(hit == mAttributeLocationByName.end(), "cannot find location of attribute '%s'", name);

            auto typeInShaderCode = mAttributeTypesByLocation.at(hit->second);
            auto typeInCPPCode    = getType<V>();
            if (typeInShaderCode != typeInCPPCode) 
            {
                auto cppName = getTypeName(typeInCPPCode);
                auto shaderName = getTypeName(typeInShaderCode);
                panic("type mismatch: attribute '%s' is declared as '%s', but in shader code, it is declared as '%s'", name, cppName, shaderName);
            }

            return Attribute<V>{glGetAttribLocation(mHandle, name.c_str())};
        }

    private:
        static void create(GLuint &handle) { handle = glCreateProgram(); }
        static void destroy(GLuint handle) { gl(DeleteProgram(handle)); }
        Handle<create, destroy> mHandle;
        std::unordered_map<GLint, GLenum> mUniformTypesByLocation, mAttributeTypesByLocation;
        std::unordered_map<std::string, GLint> mUniformLocationByName, mAttributeLocationByName;
        bool mLinked=false;
    };

}}
