#include <ray/assets/Font.hpp>
#include <cstdlib>

using namespace ray::assets;

int main()
{
    auto roboto = Font("res/fonts/Roboto-Regular.ttf", 50);

    auto index   = roboto.getGlyphIndex('A');
    auto metrics = roboto.getGlyphMetrics(index);
    auto glyph   = roboto.rasterizeGlyph(index);

    glyph.toPNG("A.png");

    return EXIT_SUCCESS;
}