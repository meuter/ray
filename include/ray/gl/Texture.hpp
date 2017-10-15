#pragma once

#include <ray/gl/AbstractTexture.hpp>
#include <ray/assets/Bitmap.hpp>
#include <string>

namespace ray { namespace gl {

    using sampler2D = sampler<GL_TEXTURE_2D>;

    class Texture : public AbstractTexture<GL_TEXTURE_2D>
    {
    protected:
        using Bitmap = assets::Bitmap;
        using Color = assets::Color;
        using u8 = unsigned char;

    public:
        Texture() { setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE); setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR); }
        Texture(int width, int height, int depth, u8* pixels=nullptr) : Texture() { load(width, height, depth, pixels); }
        Texture(int width, int height, int depth, const Color &color) : Texture() { load(width, height, depth, color); }        
        Texture(const char *filename) : Texture() { load(filename); }
        Texture(const Bitmap &bitmap) : Texture() { load(bitmap); }
        Texture(const Color &color)   : Texture() { load(color); }
        Texture(const Texture &other) = delete;
        Texture(Texture &&other) = default;

        Texture &operator=(const Texture &other) = delete;
        Texture &operator=(Texture &&other) = default;

        void load(const std::string &filename) const { load(Bitmap(filename)); }
        void load(const Bitmap &bitmap)        const { load(bitmap.width(), bitmap.height(), bitmap.depth(), bitmap.pixels()); }
        void load(const Color &color)    const { load(Bitmap(1, 1, 4, color)); }
        void load(int width, int height, int depth, const Color &color)    const { load(Bitmap(width, height, depth, color)); }        
        void load(int width, int height, int depth, const GLubyte *pixels) const { AbstractTexture::load(width, height, depth, pixels, GL_TEXTURE_2D); }
        
        void loadAt(int x, int y, const std::string &filename) const { loadAt(x, y, Bitmap(filename)); }
        void loadAt(int x, int y, const Bitmap &bitmap)        const { loadAt(x, y, bitmap.width(), bitmap.height(), bitmap.depth(), bitmap.pixels()); }
        void loadAt(int x, int y, int width, int height, int depth, const GLubyte *pixels) const;

        void resize(int width, int height, int depth) const { load(width, height, depth, nullptr); }
        
        int width()  const { return getParameter(GL_TEXTURE_WIDTH); }
        int height() const { return getParameter(GL_TEXTURE_HEIGHT); }
        int depth()  const { return getParameter(GL_TEXTURE_DEPTH); }
        int stride() const { return width() * height(); }
        int size()   const { return stride() * depth(); }
        
        int  getParameter(GLenum parameter) const;    
    };

}}
