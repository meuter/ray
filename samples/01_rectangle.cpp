#include <ray/platform/Window.hpp>
#include <ray/assets/Color.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/Shader.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::assets;

int main()
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec2 vertPosition;
        in  vec4 vertColor;
        out vec4 fragColor;
        void main() { gl_Position = vec4(vertPosition,0,1); fragColor = vertColor; }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec4 fragColor;
        out vec4 color;
        void main() { color = fragColor; }
    );
    
    auto window = Window(1920, 1080, "Rectangle Sample"); 
    auto quad   = VertexArray();
    auto shader = ShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    
    quad.bindAttribute(shader.getAttribute<vec2>("vertPosition"), VertexBuffer<f32,2>{
        -0.5f, -0.5f, // bottom left
        -0.5f,  0.5f, // top left
         0.5f, -0.5f, // bottom right
         0.5f,  0.5f  // top right
    });

    quad.bindAttribute(shader.getAttribute<vec4>("vertColor"), VertexBuffer<Color,1>{ RED, GREEN, BLUE, YELLOW }, true);

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