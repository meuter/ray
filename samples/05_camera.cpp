#include <ray/components/Movable.hpp>
#include <ray/components/Orientable.hpp>
#include <ray/entities/Camera.hpp>
#include <ray/math/Transform.hpp>
#include <ray/entities/TransformableMesh.hpp>
#include <ray/platform/Window.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/FileSystem.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::assets;
using namespace ray::components;
using namespace ray::entities;

class MeshRenderer
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec3 vertPosition;
        in  vec2 vertTexCoord;
        out vec2 fragTexCoord;
        uniform mat4 modelMatrix;
        uniform mat4 projectionMatrix;
        uniform mat4 viewMatrix;
        void main() 
        { 
            fragTexCoord = vertTexCoord;
            gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertPosition,1); 
        }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec3 fragColor;
        in  vec2 fragTexCoord;
        out vec4 color;
        uniform sampler2D diffuseTexture;
        void main() 
        {                         
            color = texture(diffuseTexture, fragTexCoord);
        }
    );

public:
    MeshRenderer(const Window &window, const Camera &camera)
    {
        shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
        texture = shader.getUniform<sampler2D>("diffuseTexture");
        modelMatrix = shader.getUniform<mat4>("modelMatrix");
        projectionMatrix = shader.getUniform<mat4>("projectionMatrix");    
        viewMatrix = shader.getUniform<mat4>("viewMatrix");
        projectionMatrix = camera.projectionMatrix();
    }

    void bind(const Mesh &mesh) const
    {
        mesh.bindPosition(shader.getAttribute<vec3>("vertPosition"));
        mesh.bindTexCoord(shader.getAttribute<vec2>("vertTexCoord"));            
    }

    void render(const Camera &camera, const TransformableMesh &mesh) const
    {
        glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.start();
        glEnable(GL_DEPTH_TEST);
        texture.set(mesh.diffuseTexture().bind(GL_TEXTURE0));
        modelMatrix.set(mesh.modelMatrix());
        viewMatrix.set(camera.viewMatrix());
        mesh.draw();
        glDisable(GL_DEPTH_TEST);
        shader.stop();
    }

private:
    ShaderProgram shader;
    Uniform<sampler2D> texture;
    Uniform<mat4> modelMatrix, projectionMatrix, viewMatrix;
};


int main()
{    
    auto window   = Window(1920, 1080, "Camera Sample");
    auto loop     = GameLoop(window, 60);
    auto camera   = Camera(43_deg, window.aspectRatio(), 0.001f, 1000.0f);
    auto renderer = MeshRenderer(window, camera);
    auto mesh     = TransformableMesh("res/mesh/teapot.obj");
    
    mesh.scale(0.05f);
    mesh.move(vec3(0,-1,0), 2);

    camera.move(camera.back(), 10);
    renderer.bind(mesh);

    loop.run([&]() 
    {   
        camera.update(window, loop.dt().count());
        renderer.render(camera, mesh);
        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });

    return EXIT_SUCCESS;
}