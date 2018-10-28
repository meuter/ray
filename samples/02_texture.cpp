#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;

int main()
{
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
        in  vec2 fragTexCoord;
        out vec4 color;
        uniform sampler2D quadTexture;
        void main() 
        {                         
            color = texture(quadTexture, fragTexCoord);
        }
    );
    
    auto window  = Window(1920, 1080, "Texture Sample");
    auto texture = Texture("res/images/awesomeface.png");
    auto shader  = ShaderProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    auto quad    = VertexArray();
    auto vbo     = VertexBuffer<f32,4>({
        // position     tex coord
        -0.5f, -0.5f,   0.0f, 1.0f, // bottom left
        -0.5f,  0.5f,   0.0f, 0.0f, // top left 
         0.5f, -0.5f,   1.0f, 1.0f, // bottom right
         0.5f,  0.5f,   1.0f, 0.0f, // top right
    });
    
    quad.bindAttributeAtOffset(0, shader.getAttribute<vec2>("vertPosition"), vbo);
    quad.bindAttributeAtOffset(2, shader.getAttribute<vec2>("vertTexCoord"), vbo);
    shader.getUniform<sampler2D>("quadTexture").set(texture.bind(GL_TEXTURE0));
    
    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        quad.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)vbo.vertexCount());

        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}