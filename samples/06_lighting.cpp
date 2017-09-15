#include <ray/platform/Window.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/FileSystem.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/entities/TransformableMesh.hpp>
#include <ray/components/Movable.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::assets;
using namespace ray::components;
using namespace ray::entities;

struct Light : public Movable
{
    Light(const vec3 &position, const Color &color) : Movable(position), color(color) {}
    Color color;
};

struct Material 
{
    Color color;
    float reflectivity, shineDamper;
};

class MeshRenderer
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec3 vertPosition;
        in  vec3 vertNormal;
        out vec3 surfaceNormal;
        out vec3 lightVector;
        out vec3 cameraVector;

        uniform mat4 modelMatrix;
        uniform mat4 projectionMatrix;
        uniform vec3 lightPosition;  

        void main() 
        { 
            vec4 worldPosition = modelMatrix * vec4(vertPosition,1);
            gl_Position = projectionMatrix * worldPosition;
            surfaceNormal = (modelMatrix * vec4(vertNormal, 0)).xyz;
            lightVector = lightPosition - worldPosition.xyz;
            cameraVector = vec3(0,0,0) - worldPosition.xyz; 
        }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec3 surfaceNormal;
        in  vec3 lightVector;
        in  vec3 cameraVector;        
        out vec4 color;

        uniform vec4 modelColor;        
        uniform vec4 lightColor;        
        uniform float reflectivity;
        uniform float shineDamper;

        void main() 
        {     
            vec3 unitNormal = normalize(surfaceNormal);
            vec3 unitLightVector = normalize(lightVector);
            vec3 unitCameraVector = normalize(cameraVector);
            vec3 reflectedLightVector = reflect(-unitLightVector, unitNormal);
            vec4 diffuse = lightColor * max(dot(unitNormal, unitLightVector), 0.2);
            vec4 specular = lightColor * reflectivity * pow(max(dot(reflectedLightVector, unitCameraVector), 0.0), shineDamper);

            color = modelColor * (diffuse + specular);
        }
    );

public:
    MeshRenderer(const Window &window)
    {
        shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
        modelMatrix = shader.getUniform<mat4>("modelMatrix");
        projectionMatrix = shader.getUniform<mat4>("projectionMatrix");    
        modelColor = shader.getUniform<vec4>("modelColor");
        lightColor = shader.getUniform<vec4>("lightColor");
        lightPosition = shader.getUniform<vec3>("lightPosition");
        reflectivity = shader.getUniform<float>("reflectivity");
        shineDamper = shader.getUniform<float>("shineDamper");

        projectionMatrix = projection(43_deg, window.aspectRatio(), 0.01f, 1000.0f);
    }

    void bind(const Mesh &mesh) const
    {
        mesh.bindPosition(shader.getAttribute<vec3>("vertPosition"));
        mesh.bindNormal(shader.getAttribute<vec3>("vertNormal"));
    }

    void render(const TransformableMesh &mesh, const Material &material, const Light &light) const
    {
        glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        modelMatrix.set(mesh.modelMatrix());
        modelColor.set(material.color);
        reflectivity.set(material.reflectivity);
        shineDamper.set(material.shineDamper);
        lightColor.set(light.color);
        lightPosition.set(light.position());
        mesh.draw();
        glDisable(GL_DEPTH_TEST);
    }

private:
    ShaderProgram shader;
    Uniform<mat4> modelMatrix, projectionMatrix;
    Uniform<vec4> modelColor, lightColor;
    Uniform<vec3> lightPosition;
    Uniform<float> reflectivity, shineDamper;
};

int main()
{    
    auto window   = Window(1920, 1080, "Lighting Sample");
    auto loop     = GameLoop(window, 60);
    auto renderer = MeshRenderer(window);
    auto mesh     = TransformableMesh("res/mesh/bunny.obj");
    auto material = Material{DARK_GRAY, 1.0f, 10.0f};
    auto light    = Light(vec3(2,2,5), YELLOW);

    mesh.moveTo(0,-0.8f,-3.5f);

    renderer.bind(mesh);

    loop.run([&]() 
    {        
        renderer.render(mesh, material, light);
        mesh.rotate(vec3(0,1,0), 2_deg);
        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });

    return EXIT_SUCCESS;
}