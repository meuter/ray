#pragma once

#include <ray/gl/Handle.hpp>
#include <ray/assets/Bitmap.hpp>
#include <ray/platform/Panic.hpp>
#include <string>

namespace ray { namespace gl {
    
    struct sampler2D { GLuint value; };        

    class Texture 
    {
    protected:
        using Bitmap = assets::Bitmap;
        using u8 = unsigned char;

    public:
        Texture() { setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE); setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR); }
        Texture(int width, int height, int depth, u8* pixels=nullptr) : Texture() { load(width, height, depth, pixels); }
        Texture(const char *filename) : Texture() { load(filename); }
        Texture(const Bitmap &bitmap) : Texture() { load(bitmap); }
        Texture(const Texture &other) = delete;
        Texture(Texture &&other) = default;

        Texture &operator=(const Texture &other) = delete;
        Texture &operator=(Texture &&other) = default;

        void load(const std::string &filename) const { load(Bitmap(filename)); }
        void load(const Bitmap &bitmap)        const { load(bitmap.width(), bitmap.height(), bitmap.depth(), bitmap.pixels()); }
        void load(int width, int height, int depth, const GLubyte *pixels) const;
        
        void loadAt(int x, int y, const std::string &filename) const { loadAt(x, y, Bitmap(filename)); }
        void loadAt(int x, int y, const Bitmap &bitmap)        const { loadAt(x, y, bitmap.width(), bitmap.height(), bitmap.depth(), bitmap.pixels()); }
        void loadAt(int x, int y, int width, int height, int depth, const GLubyte *pixels) const;

        void resize(int width, int height, int depth) const { load(width, height, depth, nullptr); }
        
        int width()  const { return getParameter(GL_TEXTURE_WIDTH); }
        int height() const { return getParameter(GL_TEXTURE_HEIGHT); }
        int depth()  const { return getParameter(GL_TEXTURE_DEPTH); }
        int stride() const { return width() * height(); }
        int size()   const { return stride() * depth(); }
        GLuint handle() const { return mHandle; }
        
        void setWrap(GLenum wrapS, GLenum wrapT) const;
        void setFilter(GLenum minFilter, GLenum magFilter) const;
        void setSwizzle(GLint r, GLint g, GLint b, GLint a) const;
        int  getParameter(GLenum parameter) const;    
        
        sampler2D bind(GLuint slotIndex=GL_TEXTURE0) const;

    private:
        static void destroy(GLuint handle) { gl(DeleteTextures(1, &handle)); }
        static void create(GLuint &handle) { gl(GenTextures(1, &handle)); }
        gl::Handle<create, destroy> mHandle;
    };

}}
