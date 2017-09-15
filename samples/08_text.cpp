#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/Print.hpp>
#include <ray/components/TextureAtlas.hpp>
#include <ray/assets/Font.hpp>
#include <unordered_map>
#include <cstdlib>

using namespace ray::assets;
using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::components;

class CachedFont : public Font
{
public:
    CachedFont(const std::string &filename, int lineHeight) : Font(filename, lineHeight), mGlyphAtlas(1) {}

    const rect2 &getGlyphTextureCoordinates(u16 index)
    {
        auto hit = mGlyphCache.find(index);
        if (hit != mGlyphCache.end()) return hit->second;
        return (mGlyphCache[index] = mGlyphAtlas.add(rasterizeGlyph(index)));
    }

    const Texture &glyphAtlas() const { return mGlyphAtlas; }

private:
    TextureAtlas mGlyphAtlas;    
    std::unordered_map<u16, rect2> mGlyphCache;    
};

class TextRenderer
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec2 vertPosition;
        in  vec2 vertTexCoord;
        out vec2 fragTexCoord;
        uniform mat4 transform;
        void main() 
        { 
            fragTexCoord = vertTexCoord;
            gl_Position = transform * vec4(vertPosition,0,1); 
        }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec3 fragColor;
        in  vec2 fragTexCoord;
        out vec4 color;
        uniform sampler2D quadTexture;
        uniform vec4 textColor;
        void main() 
        {                         
            color = texture(quadTexture, fragTexCoord) * textColor;
        }
    );

    struct Vertex { vec2 xy, uv; };
    
public:
    static constexpr auto MAX_LETTERS = 1024;
    static constexpr auto N_VERTEX_PER_LETTERS = 4;
    static constexpr auto N_FLOATS_PER_VERTEX  = sizeof(Vertex)/sizeof(float);
    static constexpr auto N_FLOATS_PER_LETTER  = N_VERTEX_PER_LETTERS*N_FLOATS_PER_VERTEX;
    static constexpr auto N_INDICES_PER_LETTER = 6;

    TextRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER)
    {        
        mVertexBuffer.reserve(MAX_LETTERS * N_FLOATS_PER_LETTER, GL_STREAM_DRAW);
        mIndexBuffer.reserve(MAX_LETTERS * N_INDICES_PER_LETTER);        
        mQuads.bindAttributeAtOffset(0, mShader.getAttribute<vec2>("vertPosition"), mVertexBuffer);
        mQuads.bindAttributeAtOffset(2, mShader.getAttribute<vec2>("vertTexCoord"), mVertexBuffer);            
        mQuads.bindIndices(mIndexBuffer);
        mTextColor = mShader.getUniform<vec4>("textColor");
        mQuadsTexture = mShader.getUniform<sampler2D>("quadTexture");
        mTransform = mShader.getUniform<mat4>("transform");

        auto mappedIndices = mIndexBuffer.map(GL_WRITE_ONLY);
        auto index = 0;
        for (auto l = 0; l < MAX_LETTERS; l += N_INDICES_PER_LETTER)        
        {
            (*mappedIndices++) = index+0; 
            (*mappedIndices++) = index+1;
            (*mappedIndices++) = index+2; 
            (*mappedIndices++) = index+1; 
            (*mappedIndices++) = index+2;
            (*mappedIndices++) = index+3; 
            index += 4;
        }
        mIndexBuffer.unmap();
    }

    static ivec2 getViewport()
    {
        int viewPort[4];
        glGetIntegerv( GL_VIEWPORT, viewPort);
        return { viewPort[2], viewPort[3] };
    }

    vec2 renderText(const vec2 &pos, CachedFont &font, const Color &color, const std::string &u8Text)
    {
        auto cursor = pos;
        auto quadVertices = reinterpret_cast<Vertex*>(mVertexBuffer.map(GL_WRITE_ONLY));
        auto nLetters = 0;
        for (auto codepoint: u8Text)
        {
            if (codepoint == '\n') cursor = { pos.x, cursor.y+font.lineHeight() };

            auto index       = font.getGlyphIndex(codepoint);
            auto metrics     = font.getGlyphMetrics(index);    
            auto bb          = metrics.boundingBox();
            auto uv          = font.getGlyphTextureCoordinates(index);
            auto topLeft     = cursor + bb.min;
            auto bottomRight = cursor + bb.max;

            (*quadVertices++) = { {bottomRight.x, topLeft.y},     {uv.max.x, uv.min.y} };
            (*quadVertices++) = { {topLeft.x,     topLeft.y},     {uv.min.x, uv.min.y} };
            (*quadVertices++) = { {bottomRight.x, bottomRight.y}, {uv.max.x, uv.max.y} };
            (*quadVertices++) = { {topLeft.x,     bottomRight.y}, {uv.min.x, uv.max.y} };

            cursor.x += metrics.advance();
            nLetters += 1;
        }     

        mVertexBuffer.unmap();

        auto viewport = getViewport();
        mTransform.set({
            2.0f/viewport.w, 0.0f, 0.0f, -1.0f,
            0.0f, -2.0f/viewport.h, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });

        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
        mQuadsTexture.set(font.glyphAtlas().bind(GL_TEXTURE0));
        mTextColor.set(color);
        mQuads.bind();
        glDrawElements(GL_TRIANGLES, N_INDICES_PER_LETTER * nLetters, GL_UNSIGNED_INT, 0);
        glDisable(GL_BLEND);

        return cursor;
    }

private:
    ShaderProgram mShader;
    VertexArray mQuads;
    VertexBuffer<f32,4> mVertexBuffer;
    Uniform<vec4> mTextColor;
    Uniform<sampler2D> mQuadsTexture;
    Uniform<mat4> mTransform;
    ElementBuffer mIndexBuffer;
};

#define FAST

int main()
{
    auto window    = Window(1920, 1080, "Text Sample");
    auto loop      = GameLoop(window, 60);    
    auto small     = CachedFont("res/fonts/Roboto-Regular.ttf", 50);
    auto big       = CachedFont("res/fonts/Roboto-Regular.ttf", 350);
    auto renderer1 = TextRenderer();
    auto renderer2 = TextRenderer();
    auto renderer3 = TextRenderer();

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
    loop.run([&]() 
    {   
        glClear(GL_COLOR_BUFFER_BIT);
        auto text = fmt("average frame time = %6.3fmsec", 1000*loop.averageFrameTime().count());
#ifdef FAST        
        renderer1.renderText(vec2(2,2), small, BLACK, text);
        renderer2.renderText(vec2(0,0), small, YELLOW, text);
        renderer3.renderText(vec2(100,100), big, RED, "Hello World!!");
#else
        // NOTE(cme): when trying to render several pieces of text using the same renderer, the second render call
        //            will have to wait for the first render call to be finished by the GPU becasue the VBO is 
        //            still being used when you try and map it to populate the next set of quads. The glMapBuffer
        //            will therefore stall. On my setup, using 3 different renderers (and therefore VBOs) is about 
        //            4x faster!! Detected using the "Instruments" built-in profiler on OSX.
        renderer1.renderText(vec2(2,2), small, BLACK, text);
        renderer1.renderText(vec2(0,0), small, YELLOW, text);
        renderer1.renderText(vec2(100,100), big, RED, "Hello World!!");
#endif        
    });

    return EXIT_SUCCESS;
}