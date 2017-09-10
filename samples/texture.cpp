#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;

/******************************************************************************/

class TexturedQuad 
{
public:
    TexturedQuad(const std::string &textureFilename) 
    {
        mTexture.load(textureFilename);
        mVertexBuffer.load({
            // position     tex coord
            -0.5f, -0.5f,   0.0f, 1.0f, // bottom left
            -0.5f,  0.5f,   0.0f, 0.0f, // top left 
             0.5f, -0.5f,   1.0f, 1.0f, // bottom right
             0.5f,  0.5f,   1.0f, 0.0f, // top right
        });
        mVertexArray.setAttributeAtOffset(0, ATTRIBUTE_2F_POSITION, mVertexBuffer);
        mVertexArray.setAttributeAtOffset(2, ATTRIBUTE_2F_TEXCOORD, mVertexBuffer);
    }
    
    void draw() const
    {
        mVertexArray.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer.count());
    }

    const Texture &texture() const { return mTexture; }
private:
    Texture mTexture;
    VertexArray mVertexArray;
    VertexBuffer<GLfloat,4> mVertexBuffer;
};

/******************************************************************************/

class QuadRenderer
{    
public:
    QuadRenderer()
    {
        mShader.attach(FragmentShader(FRAGMENT_SHADER));
        mShader.attach(VertexShader(VERTEX_SHADER));
        mShader.bind(ATTRIBUTE_2F_POSITION, "vertPosition");
        mShader.bind(ATTRIBUTE_2F_TEXCOORD, "vertTexCoord");
        mShader.link();       
        mShader.bind(mQuadTexture, "quadTexture");
    }

    void render(const TexturedQuad &quad)
    {
        mShader.use();
        mQuadTexture.set(quad.texture().bind(GL_TEXTURE0));
        quad.draw();
    }
private:
    ShaderProgram mShader;
    Uniform<sampler2D> mQuadTexture;

    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec2 vertPosition;
        in  vec2 vertTexCoord;
        out vec2 fragTexCoord;
        void main() 
        { 
            fragTexCoord = vertTexCoord;
            gl_Position = vec4(vertPosition,0,1); 
        }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec3 fragColor;
        in  vec2 fragTexCoord;
        out vec4 color;
        uniform sampler2D quadTexture;
        void main() 
        {                         
            color = texture(quadTexture, fragTexCoord);
        }
    );
};

/******************************************************************************/

int main()
{
    auto window   = Window(1920, 1080, "Texture Sample");
    auto quad     = TexturedQuad("res/images/awesomeface.png");
    auto renderer = QuadRenderer();

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.render(quad);

        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}