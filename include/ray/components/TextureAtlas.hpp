#pragma once

#include <ray/gl/Texture.hpp>
#include <ray/math/LinearAlgebra.hpp>

namespace ray { namespace components {

    class TextureAtlas : public gl::Texture
    {
    protected:
        using rect2 = math::rect2;
        using ivec2 = math::ivec2;
        using u8 = math::u8;
    public:
        TextureAtlas(int width, int height, int depth) : Texture(width, height, depth, Color(0)), mCursor{1,1}, mNextY(1) {}
        TextureAtlas(const TextureAtlas &&other) = delete;
        TextureAtlas(TextureAtlas &&other) = default;
    
        TextureAtlas &operator=(const TextureAtlas &other) = delete;
        TextureAtlas &operator=(TextureAtlas &&other) = default;
    
        rect2 add(const Bitmap &bitmap) { return add(bitmap.width(), bitmap.height(), bitmap.depth(), bitmap.pixels()); }
        rect2 add(const std::string &filename) { return add(Bitmap(filename)); }
        rect2 add(int width, int height, int depth, const u8 *pixels);
    private:
        ivec2 mCursor;
        int mNextY;
    };

}}