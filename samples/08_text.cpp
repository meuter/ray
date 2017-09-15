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
    TextRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER) 
    {        
        mVertexBuffer.load({
            // position     tex coord
            0.0f, 0.0f,   0.0f, 1.0f, // bottom left
            0.0f, 0.0f,   0.0f, 0.0f, // top left 
            0.0f, 0.0f,   1.0f, 1.0f, // bottom right
            0.0f, 0.0f,   1.0f, 0.0f, // top right
        });
        mQuad.bindAttributeAtOffset(0, mShader.getAttribute<vec2>("vertPosition"), mVertexBuffer);
        mQuad.bindAttributeAtOffset(2, mShader.getAttribute<vec2>("vertTexCoord"), mVertexBuffer);            
        mAllWhite.load(WHITE);

        mTextColor = mShader.getUniform<vec4>("textColor");
        mQuadTexture = mShader.getUniform<sampler2D>("quadTexture");
        mTransform = mShader.getUniform<mat4>("transform");
    }

    static ivec2 getViewport()
    {
        int viewPort[4];
        glGetIntegerv( GL_VIEWPORT, viewPort);
        return { viewPort[2], viewPort[3] };
    }

    static std::u16string toUTF16(const std::string &u8Text)
    {
        return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}.from_bytes(&u8Text[0]);    
    }

    vec2 renderText(const vec2 &pos, const BakedFont &font, const Color &color, const std::u16string &u16Text)
    {
        auto cursor = pos;
        for (auto codepoint: u16Text)
            cursor = renderCodepoint(cursor, font, color, codepoint);
        return cursor;
    }

    vec2 renderCodepoint(const vec2 &pos, const BakedFont &font, const Color &color, int codepoint)
    {
        auto glyph = font.getGlyph(codepoint);

        Vertex *quadVertices = (Vertex *)mVertexBuffer.map(GL_WRITE_ONLY);

        auto topLeft     = pos + glyph.bb.min;
        auto bottomRight = pos + glyph.bb.max;

        quadVertices[0] = { {bottomRight.x, topLeft.y},     {glyph.uv.max.x, glyph.uv.min.y} };
        quadVertices[1] = { {topLeft.x,     topLeft.y},     {glyph.uv.min.x, glyph.uv.min.y} };
        quadVertices[2] = { {bottomRight.x, bottomRight.y}, {glyph.uv.max.x, glyph.uv.max.y} };
        quadVertices[3] = { {topLeft.x,     bottomRight.y}, {glyph.uv.min.x, glyph.uv.max.y} };

        auto viewport = getViewport();
        mTransform.set({
            2.0f/viewport.w, 0.0f, 0.0f, -1.0f,
            0.0f, -2.0f/viewport.h, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });

        mVertexBuffer.unmap();

        mQuadTexture.set(font.texture().bind(GL_TEXTURE0));
        mTextColor.set(color);
        mQuad.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        return { pos.x + glyph.advance, pos.y };
    }

private:
    ShaderProgram mShader;
    VertexArray mQuad;
    VertexBuffer<f32,4> mVertexBuffer;
    Texture mAllWhite;
    Uniform<vec4> mTextColor;
    Uniform<sampler2D> mQuadTexture;
    Uniform<mat4> mTransform;
};


int main()
{
    auto window   = Window(1920, 1080, "Font Sample");
    auto loop     = GameLoop(window, 60);    
    auto melso    = BakedFont("res/fonts/Roboto-Regular.ttf", 20);
    auto renderer = TextRenderer();
    
    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    loop.run([&]() 
    {   
        glClear(GL_COLOR_BUFFER_BIT);

        auto text = fmt("average frame time = %1%msec", 1000*loop.averageFrameTime().count());

        renderer.renderText(vec2(0,0), melso, YELLOW, renderer.toUTF16(text));
    });

    return EXIT_SUCCESS;
}