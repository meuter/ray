#include <ray/assets/Bitmap.hpp>
#include <ray/platform/Panic.hpp>

// NOTE(cme): silence two unused parameters
//            fixed in https://github.com/nothings/stb/pull/662
#if defined(__clang__)
#   pragma clang diagnostic push 
#   pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#   pragma GCC diagnostic push 
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#undef STB_IMAGE_IMPLEMENTATION

#if defined(__clang__)
#   pragma clang diagnostic pop
#elif defined(__GNUC__)
#   pragma GCC diagnostic pop
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

namespace ray { namespace assets {

    Bitmap::Bitmap(const std::string &filename) 
    {
        mPixels = stbi_load(filename.c_str(), &mWidth, &mHeight, &mDepth, 0);
        panicif(mPixels == nullptr, "could not load bitmap '%s'", filename);
    }

    Bitmap::Bitmap(int width, int height, int depth, const Color &color) : Bitmap(width, height, depth)
    {
        const auto end = mPixels + size();
        for (auto pixel = mPixels; pixel < end; pixel += depth)
            std::memcpy(pixel, &color, depth);
    }

    Bitmap::Bitmap(Bitmap &&other)
    {
        (*this) = std::move(other);
    }

    Bitmap &Bitmap::operator=(Bitmap &&other)
    {
        mPixels = other.mPixels;
        mHeight = other.mHeight;
        mWidth = other.mWidth;
        mDepth = other.mDepth;
        other.mPixels = nullptr;
        other.mHeight = other.mWidth = other.mDepth = 0;
        return (*this);
    }

    Bitmap::~Bitmap() 
    {
        free(mPixels);
    }

    void Bitmap::resize(int width, int height, int depth)
    {
        mWidth = width;
        mHeight = height;
        mDepth = depth;
        free(mPixels);
        mPixels = reinterpret_cast<uint8_t*>(malloc(width * height * depth));
        memset(mPixels, 0x00, width * height * depth);
        panicif(mPixels == nullptr, "could not allocate buffer for bitmap");
    }

    void Bitmap::toPNG(const std::string &filename) const
    {
        stbi_write_png(filename.c_str(), mWidth, mHeight, mDepth, mPixels, stride());
    }

    void Bitmap::toBMP(const std::string &filename) const
    {
        stbi_write_bmp(filename.c_str(), mWidth, mHeight, mDepth, mPixels);
    }

    void Bitmap::toJPG(const std::string &filename, int quality) const
    {
        stbi_write_jpg(filename.c_str(), mWidth, mHeight, mDepth, mPixels, quality);
    }

}}
