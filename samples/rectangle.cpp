#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;

/******************************************************************************/

class Quad 
{
public:
    Quad() 
    {
        mVertexBuffer.load({
            -0.5f, -0.5f,
            -0.5f,  0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
        });
        mVertexArray.setAttribute(ATTRIBUTE_2F_POSITION, mVertexBuffer);
    }

    void draw() const
    {
        mVertexArray.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, mVertexBuffer.count());
    }

private:
    VertexArray mVertexArray;
    VertexBuffer<2, f32> mVertexBuffer;
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
        mShader.link();       
    }

    void render(const Quad &quad)
    {
        mShader.use();
        quad.draw();
    }
private:
    ShaderProgram mShader;

    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec2 vertPosition;
        void main() { gl_Position = vec4(vertPosition,0,1); }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec3 fragColor;
        out vec4 color;
        void main() { color = vec4(1,1,1,1); }
    );
};

/******************************************************************************/

int main()
{
    auto window = Window(1920, 1080, "Rectangle Sample");
    window.makeContextCurrent();
    window.loadGLExtensions();
    window.swapInterval(0);
    
    auto quad     = Quad();
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