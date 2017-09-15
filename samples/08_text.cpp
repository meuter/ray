#include <ray/assets/Font.hpp>
#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/components/BakedFont.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/Print.hpp>
#include <cstdlib>
#include <codecvt>

using namespace ray::assets;
using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::components;
    
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
        mQuad.bindAttributeAtOffset(0, mShader.getAttribute<vec2>("vertPosition"), mVertexBuffer);
        mQuad.bindAttributeAtOffset(2, mShader.getAttribute<vec2>("vertTexCoord"), mVertexBuffer);            
        mQuad.bindIndices(mIndexBuffer);
        mTextColor = mShader.getUniform<vec4>("textColor");
        mQuadTexture = mShader.getUniform<sampler2D>("quadTexture");
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

    vec2 renderText(const vec2 &pos, BakedFont &font, const Color &color, const std::string &u8Text)
    {
        auto cursor = pos;

        auto quadVertices = reinterpret_cast<Vertex*>(mVertexBuffer.map(GL_WRITE_ONLY));
        auto nLetters = 0;
        for (auto codepoint: u8Text)
        {
            auto glyph = font.getGlyph(codepoint);
            auto topLeft     = cursor + glyph.bb.min;
            auto bottomRight = cursor + glyph.bb.max;

            (*quadVertices++) = { {bottomRight.x, topLeft.y},     {glyph.uv.max.x, glyph.uv.min.y} };
            (*quadVertices++) = { {topLeft.x,     topLeft.y},     {glyph.uv.min.x, glyph.uv.min.y} };
            (*quadVertices++) = { {bottomRight.x, bottomRight.y}, {glyph.uv.max.x, glyph.uv.max.y} };
            (*quadVertices++) = { {topLeft.x,     bottomRight.y}, {glyph.uv.min.x, glyph.uv.max.y} };

            cursor.x += glyph.advance;
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
        mQuadTexture.set(font.texture().bind(GL_TEXTURE0));
        mTextColor.set(color);
        mQuad.bind();
        glDrawElements(GL_TRIANGLES, N_INDICES_PER_LETTER * nLetters, GL_UNSIGNED_INT, 0);
        glDisable(GL_BLEND);

        return cursor;
    }


private:
    ShaderProgram mShader;
    VertexArray mQuad;
    VertexBuffer<f32,4> mVertexBuffer;
    Uniform<vec4> mTextColor;
    Uniform<sampler2D> mQuadTexture;
    Uniform<mat4> mTransform;
    ElementBuffer mIndexBuffer;
};


int main()
{
    auto window   = Window(1920, 1080, "Text Sample");
    auto loop     = GameLoop(window, 60);    
    auto melso    = BakedFont("res/fonts/Roboto-Regular.ttf", 30);
    auto renderer = TextRenderer();
    
    int maxsize;
    glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE, &maxsize);
    fprintln("max texture size = %1%", maxsize);

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
    loop.run([&]() 
    {   
        glClear(GL_COLOR_BUFFER_BIT);
        renderer.renderText(vec2(0,0), melso, YELLOW, fmt("average frame time = %6.3fmsec", 1000*loop.averageFrameTime().count()));
    });

    return EXIT_SUCCESS;
}