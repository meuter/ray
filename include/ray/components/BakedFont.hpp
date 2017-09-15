#pragma once

#include <ray/assets/Font.hpp>
#include <ray/assets/Bitmap.hpp>
#include <ray/components/TextureAtlas.hpp>
#include <unordered_map>

namespace ray { namespace components {

    class BakedFont : public assets::Font
    {
    protected:
        using rect2 = math::rect2;
        using irect2 = math::irect2;
        using ivec2 = math::ivec2;
        using u8 = math::u8;
        using u16 = math::u16;

    public:
        struct Glyph { rect2 uv; rect2 bb; int advance; };
        enum RasterType {  NORMAL_RASTER, SDF_RASTER };

        BakedFont(const std::string &filename, int pixelHeight, const RasterType &rasterType=NORMAL_RASTER)
            : Font(filename, pixelHeight), mGlyphAtlas(1), mRasterType(rasterType)
        { }

        const Glyph &getGlyph(u16 codepoint) 
        { 
            if (!hasGlyph(codepoint)) bakeGlyph(codepoint);
            return mGlyphCache.at(codepoint);
        }

        bool hasGlyph(u16 codepoint) const 
        { 
            return mGlyphCache.find(codepoint) != mGlyphCache.end(); 
        }

        void bakeGlyph(u16 codepoint)
        {
            auto glyphBitmap = assets::Bitmap();
            auto glyphIndex  = getGlyphIndex(codepoint);
            auto metrics     = getGlyphMetrics(glyphIndex);
            auto bb          = metrics.boundingBox();
            
            if (mRasterType == SDF_RASTER)
            {
                int padding = lineHeight() * 0.1;
                int slope   = 255.0f / (lineHeight() * 0.1f);
                glyphBitmap = rasterizeGlyphSDF(glyphIndex, padding, 255, slope);
            }
            else
            {
                glyphBitmap = rasterizeGlyph(glyphIndex);
            }

            auto uv = mGlyphAtlas.add(glyphBitmap);        
            auto glyphDims = ivec2{ glyphBitmap.width(), glyphBitmap.height() };
            
            mGlyphCache[codepoint] = {
                uv,
                math::rect_cast<float>(irect2{ bb.min, bb.min+glyphDims }),// TODO(cme): why can't I use bb.max here?? does not make sense?
                metrics.advance()
            };        
        }
        
        const gl::Texture &texture() const { return mGlyphAtlas; }
            
    private:
        TextureAtlas mGlyphAtlas;
        std::unordered_map<u16, Glyph> mGlyphCache;
        RasterType mRasterType;
    };

}}