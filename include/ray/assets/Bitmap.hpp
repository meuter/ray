#pragma once

#include <string>
#include <ray/assets/Color.hpp>

namespace ray { namespace assets {

    class Bitmap 
    {
        using u8 = unsigned char;
    public:
        Bitmap() : mPixels(nullptr), mWidth(0), mHeight(0), mDepth(0) {}
        Bitmap(int width, int height, int depth, u8 *pixels) : mPixels(pixels), mWidth(width), mHeight(height), mDepth(depth) {}
        Bitmap(int width, int height, int depth) : mPixels(nullptr) { resize(width, height, depth); }
        Bitmap(int width, int height, int depth, const Color &color);
        Bitmap(const std::string &filename);
        Bitmap(const Bitmap &other) = delete;
        Bitmap(Bitmap &&other);
        ~Bitmap();

        Bitmap &operator=(Bitmap &&other);
        Bitmap &operator=(const Bitmap &&other) = delete;

        int width()        const { return mWidth; }
        int height()       const { return mHeight; }
        int depth()        const { return mDepth; }
        int stride()       const { return mWidth * mDepth; }
        int size()         const { return mWidth * mHeight * mDepth; }
        const u8 *pixels() const { return mPixels; }

        void resize(int width, int height, int depth);
        void toPNG(const std::string &filename) const;
        void toBMP(const std::string &filename) const;
        void toJPG(const std::string &filename, int quality=80) const;
        
    private:
        unsigned char *mPixels;
        int mWidth, mHeight, mDepth;
    };

}}