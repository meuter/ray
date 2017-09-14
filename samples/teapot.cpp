#include <ray/rendering/Mesh.hpp>
#include <ray/platform/Window.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/FileSystem.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/math/Transform.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::assets;
using namespace ray::rendering;

class MeshRenderer
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec3 vertPosition;
        in  vec2 vertTexCoord;
        out vec2 fragTexCoord;
        uniform mat4 modelMatrix;
        uniform mat4 projectionMatrix;
        void main() 
        { 
            fragTexCoord = vertTexCoord;
            gl_Position = projectionMatrix * modelMatrix * vec4(vertPosition,1); 
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

public:
    MeshRenderer(const Window &window)
    {
        shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
        texture = shader.getUniform<sampler2D>("quadTexture");
        modelMatrix = shader.getUniform<mat4>("modelMatrix");
        projectionMatrix = shader.getUniform<mat4>("projectionMatrix");    

        projectionMatrix = projection(43_deg, window.aspectRatio(), 0.01f, 1000.0f);
    }

    void bind(const Mesh &mesh) const
    {
        mesh.bindPosition(shader.getAttribute<vec3>("vertPosition"));
        mesh.bindTexCoord(shader.getAttribute<vec2>("vertTexCoord"));            
    }

    void render(const Mesh &mesh) const
    {
        glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        texture.set(mesh.diffuseTexture().bind(GL_TEXTURE0));
        modelMatrix.set(mesh.modelMatrix());
        mesh.draw();
        glDisable(GL_DEPTH_TEST);
    }

private:
    ShaderProgram shader;
    Uniform<sampler2D> texture;
    Uniform<mat4> modelMatrix, projectionMatrix, viewMatrix;
};

int main()
{    
    auto window   = Window(1920, 1080, "Teapot Sample");
    auto loop     = GameLoop(window, 60);
    auto renderer = MeshRenderer(window);
    auto mesh     = Mesh("res/mesh/teapot.obj");

    mesh.scale(0.05f);
    mesh.moveTo(0,-2.0f,-10);
    mesh.rotate(vec3(1,0,0), 10_deg);

    renderer.bind(mesh);

    loop.run([&]() 
    {
        renderer.render(mesh);
        mesh.rotate(vec3(0,1,0), 2_deg);
        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });

    return EXIT_SUCCESS;
}