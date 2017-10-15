#include <ray/gl/Texture.hpp>
  
namespace ray { namespace gl {

    void Texture::loadAt(int x, int y, int width, int height, int depth, const GLubyte *pixels) const
    {
        bind();
        gl(PixelStorei(GL_UNPACK_ROW_LENGTH, width*depth));
        gl(PixelStorei(GL_UNPACK_ALIGNMENT, 1));
        switch(depth)
        {
        case 1:
            gl(TexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RED, GL_UNSIGNED_BYTE, pixels));
            break;
        case 2:
            gl(TexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RG, GL_UNSIGNED_BYTE, pixels));
            break;
        case 3:
            gl(TexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels));
            break;
        case 4:   
            gl(TexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels));
            break;
        default:
            panic("unexpected number of channels '%d'", depth);
        }
        gl(PixelStorei(GL_UNPACK_ROW_LENGTH, 0));
        gl(PixelStorei(GL_UNPACK_ALIGNMENT, 4));
        gl(GenerateMipmap(GL_TEXTURE_2D));
    }

    int Texture::getParameter(GLenum parameter) const
    {
        bind();
        int result;
        gl(GetTexLevelParameteriv(GL_TEXTURE_2D, 0, parameter, &result));
        return result;        
    }
}}