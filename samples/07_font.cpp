#include <ray/assets/Font.hpp>
#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <cstdlib>

using namespace ray::assets;
using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;



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

    int renderCodepoint(const vec2 &pos, const Font &font, const Color &color, int codepoint)
    {
        auto glyph = font.rasterizeCodepoint(codepoint);
        auto glyphTex = Texture(glyph);
        auto metrics = font.getCodepointMetrics(codepoint);
        auto offset = metrics.boundingBox().min;

        Vertex *quadVertices = (Vertex *)mVertexBuffer.map(GL_WRITE_ONLY);

        quadVertices[0].xy = vec2(pos.x+offset.x,                   pos.y+offset.y+glyph.height());
        quadVertices[1].xy = vec2(pos.x+offset.x,                   pos.y+offset.y);
        quadVertices[2].xy = vec2(pos.x+offset.x+glyph.width(),     pos.y+offset.y+glyph.height());
        quadVertices[3].xy = vec2(pos.x+offset.x+glyph.width(),     pos.y+offset.y);

        auto viewport = getViewport();
        mTransform.set({
            2.0f/viewport.w, 0.0f, 0.0f, -1.0f,
            0.0f, -2.0f/viewport.h, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });

        mVertexBuffer.unmap();

        mQuadTexture.set(glyphTex.bind(GL_TEXTURE0));
        mTextColor.set(color);
        mQuad.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        return metrics.advance();
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
    auto melso    = Font("res/fonts/Roboto-Regular.ttf", 500);
    auto renderer = TextRenderer();
    
    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        auto x = 300;
        auto y = 200;
        x += renderer.renderCodepoint(vec2(x,y), melso, RED, 'H');
        x += renderer.renderCodepoint(vec2(x,y), melso, BLUE, 'e');
        x += renderer.renderCodepoint(vec2(x,y), melso, GREEN, 'l');
        x += renderer.renderCodepoint(vec2(x,y), melso, YELLOW, 'l');
        x += renderer.renderCodepoint(vec2(x,y), melso, PINK, 'o');
        x += renderer.renderCodepoint(vec2(x,y), melso, TEAL, '!');
        
        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}