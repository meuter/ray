#include <ray/components/TextureAtlas.hpp>

using namespace ray::math;

namespace ray { namespace components {

    rect2 TextureAtlas::add(int width, int height, int depth, const u8 *pixels)
    {
        panicif(depth != this->depth(), "not the same depth");
        
        if (mCursor.x + width > this->width())   mCursor = ivec2{1, mNextY};
        if (mCursor.y + height > this->height()) panic("atlas (%d,%d) too small for bitmap (%d,%d)", this->width(), this->height(), width, height);
        
        loadAt(mCursor.x, mCursor.y, width, height, depth, pixels);
        
        auto min = mCursor;
        auto max = min + ivec2{width, height};
        
        mNextY = std::max(mNextY, mCursor.y + height);
        mCursor.x += width + 1;
        
        return rect2 { 
            { (float)min.x / (float)this->width(), (float)min.y / (float)this->height() },
            { (float)max.x / (float)this->width(), (float)max.y / (float)this->height() }
        };
    }

}}