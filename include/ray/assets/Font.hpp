#pragma once

#include <ray/assets/Bitmap.hpp>
#include <ray/math/LinearAlgebra.hpp>
#include <vector>

namespace ray { namespace assets {

    class Font
    {
    public:
        class GlyphMetrics
        {
        public:
            int   advance() const { return mAdvance; }
            int   leftSideBearing() const { return mBoundingBox.min.x; }        
            int   rightSideBearing() const { return mAdvance - width() - leftSideBearing(); }        
            int   width() const { return mBoundingBox.size().w; }
            int   height() const { return mBoundingBox.size().h; }
            const math::rect2 &boundingBox() const { return mBoundingBox; }
        private:
            math::rect2 mBoundingBox;
            int mAdvance;
            friend class Font;
        };

        Font(const std::string &filename, int lineHeight);

        int lineHeight() const { return mLineHeight; }
        int ascent() const  { return mAscent; }
        int descent() const { return mDescent; }
        int lineGap() const { return mLineHeight - mAscent - mDescent; }
        float scale() const { return mScale; }

        int getGlyphIndex(int codepoint) const;

        bool hasKerning() const;
        int codepointKerning(int codepoint1, int codepoint2) const;
        int glyphKerning(int glyphIndex1, int glyphIndex2) const;

        GlyphMetrics getGlyphMetrics(int glyphIndex) const;
        GlyphMetrics getCodepointMetrics(int codepoint) const;

        Bitmap rasterizeGlyph(int glyphIndex) const;
        Bitmap rasterizeCodepoint(int codepoint) const;
        Bitmap rasterizeGlyphSDF(int glyphIndex, int padding, math::u8 insideValue, float distanceSlope) const;
        Bitmap rasterizeCodepointSDF(int codepoint, int padding, math::u8 insideValue, float distanceSlope) const;
    
    private:
        std::vector<math::u8> mInfo;
        std::vector<math::u8> mData;
        int mLineHeight, mDescent, mAscent;
        float mScale;
    };

}}