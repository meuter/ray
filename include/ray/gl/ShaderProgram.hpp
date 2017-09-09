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
            
            // NOTE: collect info about uniforms for later binding        
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

            // NOTE: typecheck bound attribute
            GLint attributeCount = 0;
            gl(GetProgramiv(mHandle, GL_ACTIVE_ATTRIBUTES, &attributeCount));
    
            for (GLint attributeIndex = 0; attributeIndex < attributeCount; attributeIndex++)
            {
                char attributeName[512];
                GLenum typeInShaderCode;
                GLint attributeSize;
    
                gl(GetActiveAttrib(mHandle, (GLuint)attributeIndex, sizeof(attributeName), nullptr, &attributeSize, &typeInShaderCode, attributeName));

                auto hit = mAttributeTypeByName.find(attributeName);
                panicif(hit == mAttributeTypeByName.end(), "attribute '%s' was not bound before linking", attributeName);
                auto typeInCPPCode = hit->second;
                if (typeInCPPCode != typeInShaderCode)
                {
                    auto cppName = getTypeName(typeInCPPCode);
                    auto shaderName = getTypeName(typeInShaderCode);
                    panic("type mismatch: attribute '%s' is declared as '%s', but in shader code, it is declared as '%s'", attributeName, cppName, shaderName);
                }
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
            
            auto typeInShaderCode = mUniformTypesByLocation.at(uniform.mLocation);
            auto typeInCPPCode    = uniform.type();
            if (typeInShaderCode != typeInCPPCode) 
            {
                auto cppName = getTypeName(typeInCPPCode);
                auto shaderName = getTypeName(typeInShaderCode);
                panic("type mismatch: uniform '%s' is declared as '%s', but in shader code, it is declared as '%s'", name, cppName, shaderName);
            }
        }

        template<typename V, typename T>
        void bind(Attribute<V, T> attribute, const std::string &name)
        {
            panicif(mLinked, "attrbiutes must be bound before the program has been linked");
            gl(BindAttribLocation(mHandle, attribute.mLocation, name.c_str()));
            mAttributeTypeByName[name] = attribute.type();
        }

    private:
        static void create(GLuint &handle) { handle = glCreateProgram(); }
        static void destroy(GLuint handle) { gl(DeleteProgram(handle)); }
        Handle<create, destroy> mHandle;
        std::unordered_map<GLint, GLenum> mUniformTypesByLocation;
        std::unordered_map<std::string, GLint> mUniformLocationByName, mAttributeTypeByName;
        bool mLinked=false, mUniformsCollected=false;
    };

}}
