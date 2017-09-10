#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;

static constexpr auto VERTEX_SHADER = GLSL(330, 
    in  vec2 vertPosition;
    void main() { gl_Position = vec4(vertPosition,0,1); }
);

static constexpr auto FRAGMENT_SHADER = GLSL(330,
    in  vec3 fragColor;
    out vec4 color;
    void main() { color = vec4(1,1,1,1); }
);

int main()
{
    auto window   = Window(1920, 1080, "Rectangle Sample"); 
    auto quadBuf  = VertexBuffer<2, float>{
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    };
    auto quad     = VertexArray();
    quad.setAttribute(ATTRIBUTE_2F_POSITION, quadBuf);

    auto shader = ShaderProgram();
    shader.attach(VertexShader(VERTEX_SHADER));
    shader.attach(FragmentShader(FRAGMENT_SHADER));
    shader.bind(ATTRIBUTE_2F_POSITION, "vertPosition");
    shader.link();       

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        quad.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}