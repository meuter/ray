#include <ray/glfw/glfw3.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::glfw;
using namespace ray::gl;

/******************************************************************************/

class Quad : public VertexArray 
{
public:
    Quad() : VertexArray()
    {
        setVertexAttribute(ATTRIBUTE_POSITION, 2, {
            -0.5f, -0.5f,
            -0.5f,  0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
        });
    }

    void draw() const
    {
        bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        unbind();
    }
};

/******************************************************************************/

class QuadRenderer
{    
public:
    QuadRenderer()
    {
        mShader.attach(FragmentShader(FRAGMENT_SHADER));
        mShader.attach(VertexShader(VERTEX_SHADER));
        mShader.bind(ATTRIBUTE_POSITION, "vertPosition");
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
    auto window = Window(1920, 1080, "Window Title");
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