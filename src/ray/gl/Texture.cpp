#include <ray/gl/Texture.hpp>
  
namespace ray { namespace gl {

    void Texture::load(int width, int height, int depth, const GLubyte *pixels) const
    {
        bind();
        switch(depth)
        {
        case 1:
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);        
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
            setSwizzle(GL_ONE, GL_ONE, GL_ONE, GL_RED);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);            
            break;
        case 2:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, pixels);
            setSwizzle(GL_RED, GL_RED, GL_RED, GL_GREEN);
            break;
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ONE);
            break;
        case 4:   
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA);
            break;
        default:
            panic("unexpected number of channels '%d'", depth);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }

        
    void Texture::loadAt(int x, int y, int width, int height, int depth, const GLubyte *pixels) const
    {
        bind();
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width*depth);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        switch(depth)
        {
        case 1:
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels);
            break;
        case 2:
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RG, GL_UNSIGNED_BYTE, pixels);
            break;
        case 3:
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
            break;
        case 4:   
            glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
            break;
        default:
            panic("unexpected number of channels '%d'", depth);
        }
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture::setWrap(GLenum wrapS, GLenum wrapT) const
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    }

    void Texture::setFilter(GLenum minFilter, GLenum magFilter) const
    {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    void Texture::setSwizzle(GLint r, GLint g, GLint b, GLint a) const
    {
        bind();
        GLint swizzleMask[] = { r, g, b, a };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }
        
    int Texture::getParameter(GLenum parameter) const
    {
        bind();
        int result;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, parameter, &result);
        return result;        
    }

    sampler2D Texture::bind(GLuint slotIndex) const 
    {
        glActiveTexture(slotIndex);
        glBindTexture(GL_TEXTURE_2D, mHandle); 
        return sampler2D{slotIndex};
    }
}}