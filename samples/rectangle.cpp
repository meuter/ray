#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;

int main()
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec2 vertPosition;
        void main() { gl_Position = vec4(vertPosition,0,1); }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        out vec4 color;
        void main() { color = vec4(1,1,1,1); }
    );
    
    auto window = Window(1920, 1080, "Rectangle Sample"); 
    auto quad   = VertexArray();
    auto shader = ShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    
    quad.setAttribute(shader.getAttribute<vec2,float>("vertPosition"), VertexBuffer<2, float>{
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f
    });

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        quad.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}