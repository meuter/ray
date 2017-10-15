#pragma once

#include <ray/gl/Handle.hpp>
#include <ray/platform/Panic.hpp>

namespace ray { namespace gl {
    
    template<GLenum textureType>
    struct sampler { GLuint value; };

    template<GLenum textureType>
    class AbstractTexture
    {
    protected:
        static constexpr auto TEXTURE_TYPE = textureType;        

    public:
        GLuint handle() const { return mHandle; }
        
        auto bind(GLuint slotIndex=GL_TEXTURE0) const
        {
            gl(ActiveTexture(slotIndex));
            gl(BindTexture(GL_TEXTURE_2D, mHandle));
            return sampler<TEXTURE_TYPE>{GL_TEXTURE0-slotIndex};
        }

        void setSwizzle(GLint r, GLint g, GLint b, GLint a) const
        {
            bind();
            GLint swizzleMask[] = { r, g, b, a };
            gl(TexParameteriv(textureType, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
        }    

        void setFilter(GLenum minFilter, GLenum magFilter) const
        {
            bind();
            gl(TexParameteri(textureType, GL_TEXTURE_MIN_FILTER, minFilter));
            gl(TexParameteri(textureType, GL_TEXTURE_MAG_FILTER, magFilter));    
        }
    
        void setWrap(GLenum wrapS, GLenum wrapT, GLenum wrapR=0) const
        {
            bind();
            gl(TexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapS));	
            gl(TexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapT));    
            if (wrapR) gl(TexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapR));    
        }

        void load(int width, int height, int depth, const GLubyte *pixels, GLenum target=TEXTURE_TYPE, bool generateMipmap=true) const
        {
            bind();
            switch(depth)
            {
            case 1:
                gl(PixelStorei(GL_UNPACK_ALIGNMENT, 1));
                gl(TexImage2D(target, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels));
                setSwizzle(GL_ONE, GL_ONE, GL_ONE, GL_RED);
                gl(PixelStorei(GL_UNPACK_ALIGNMENT, 4));
                break;
            case 2:
                gl(TexImage2D(target, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels));
                setSwizzle(GL_RED, GL_RED, GL_RED, GL_GREEN);
                break;
            case 3:
                gl(TexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels));
                setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ONE);
                break;
            case 4:   
                gl(TexImage2D(target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
                setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA);
                break;
            default:
                panic("unexpected number of channels '%d'", depth);                
            }
            if (generateMipmap) gl(GenerateMipmap(TEXTURE_TYPE));
        }
    private:
        static void destroy(GLuint handle) { gl(DeleteTextures(1, &handle)); }
        static void create(GLuint &handle) { gl(GenTextures(1, &handle)); }
        gl::Handle<create, destroy> mHandle;
    };
}}
