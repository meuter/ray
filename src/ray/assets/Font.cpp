#include <ray/assets/Font.hpp>
#include <ray/platform/Panic.hpp>
#include <fstream>
#include <ray/platform/FileSystem.hpp>

#define STB_TRUETYPE_IMPLEMENTATION 
#include <stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION 

namespace fs = ray::platform::fs;

using namespace ray::math;

namespace ray { namespace assets {

    Font::Font(const std::string &filename, int lineHeight) : mInfo(sizeof(stbtt_fontinfo)), mLineHeight(lineHeight)
    {
        using namespace ray::platform;
        panicif(!fs::exists(filename), "attempting to open '"+filename+"' which does not exist");
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        auto fontInfo = reinterpret_cast<stbtt_fontinfo*>(mInfo.data());

        mData.resize(size);
        panicif(!file.read((char*)mData.data(), size), "could not read font file");
        panicif(!stbtt_InitFont(fontInfo, mData.data(), 0), "could not initialize font info");
        mScale = stbtt_ScaleForPixelHeight(fontInfo, mLineHeight);
        stbtt_GetFontVMetrics(fontInfo, &mAscent, &mDescent, nullptr);    
        mAscent *= mScale;
        mDescent *= -mScale;
    }

    Font::GlyphMetrics Font::getGlyphMetrics(int glyphIndex) const
    {
        irect2 bb;
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());        
        GlyphMetrics metrics;
        stbtt_GetGlyphHMetrics(fontInfo, glyphIndex, &metrics.mAdvance, nullptr);
        stbtt_GetGlyphBitmapBox(fontInfo, glyphIndex, mScale, mScale, &bb.min.x, &bb.min.y, &bb.max.x, &bb.max.y);
        metrics.mBoundingBox.min = bb.min + vec2(0, ascent());
        metrics.mBoundingBox.max = bb.max + vec2(0, ascent());
        metrics.mAdvance *= mScale;
        return metrics;
    }

    Font::GlyphMetrics Font::getCodepointMetrics(int codepoint) const
    {
        return getGlyphMetrics(getGlyphIndex(codepoint));
    }

    int Font::getGlyphIndex(int codepoint) const
    {
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());                
        return stbtt_FindGlyphIndex(fontInfo, codepoint);
    }
    
    Bitmap Font::rasterizeGlyph(int glyphIndex) const
    {
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());                        
        static const int depth = 1;
        int width, height;
        auto pixels = stbtt_GetGlyphBitmapSubpixel(fontInfo, mScale, mScale, 0, 0, glyphIndex, &width, &height, nullptr, nullptr);
        return pixels ? Bitmap(width, height, depth, pixels) : Bitmap(1,1,1,Color(0));
    }

    Bitmap Font::rasterizeCodepoint(int codepoint) const
    {
        return rasterizeGlyph(getGlyphIndex(codepoint));
    }

    Bitmap Font::rasterizeGlyphSDF(int glyphIndex, int padding, u8 insideValue, float distanceSlope) const
    {
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());                                
        static const int depth = 1;
        int width, height;
        auto pixels = stbtt_GetGlyphSDF(fontInfo, mScale, glyphIndex, padding, insideValue, distanceSlope, &width, &height, nullptr, nullptr);
        return pixels ? Bitmap(width, height, depth, pixels) : Bitmap(1,1,1,Color(0));
    }

    Bitmap Font::rasterizeCodepointSDF(int codepoint, int padding, u8 insideValue, float distanceSlope) const
    {
        return rasterizeGlyphSDF(getGlyphIndex(codepoint), padding, insideValue, distanceSlope);
    }

    bool Font::hasKerning() const
    {
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());                                        
        return (fontInfo->kern != 0);
    }
    
    int Font::codepointKerning(int codepoint1, int codepoint2) const
    {
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());                                        
        return stbtt_GetCodepointKernAdvance(fontInfo, codepoint1, codepoint2);
    }

    int Font::glyphKerning(int glyphIndex1, int glyphIndex2) const
    {
        auto fontInfo = reinterpret_cast<const stbtt_fontinfo*>(mInfo.data());                                        
        return stbtt_GetGlyphKernAdvance(fontInfo, glyphIndex1, glyphIndex2);
    }


}}
