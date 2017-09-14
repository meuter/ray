#include <ray/rendering/Mesh.hpp>
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
using namespace ray::rendering;

class Camera 
{
public:
    Camera(rad fovy, float aspect, float n, float f) : mProjectionMatrix(projection(fovy, aspect, n, f)), mPosition{0,0,0}, mOrientation{0,1,0,0} 
    {
        // NOTE(cme): from "Real-Time Renderingf 3rd Edition", Figure 4,19: the perspective projection matrix
        //             maps the view frustum (pointed towards the negative z-axis) to the canonical view volume 
        //             positive z gets out of the screen => we need to orient the camera so that the front 
        //             points towards the screen, otherwise the front(), back(), left() and right() will be 
        //             reversed => this is achieved by setting the orientation to (0,1,0,0)
    }

    mat4 projectionMatrix() const  { return mProjectionMatrix; }

    mat4 viewMatrix() const 
    {
        auto inverseTranslation = translation(-mPosition);
        auto inverseRotation = rotation(conjugate(mOrientation));

        return inverseTranslation * inverseRotation;
    }

    void update(const Window &window, float speed=0.1f)
    {
        if (window.isKeyHeld(Key::KEY_UP))        move(mOrientation.front(), speed);
        if (window.isKeyHeld(Key::KEY_DOWN))      move(mOrientation.back(), speed);
        if (window.isKeyHeld(Key::KEY_LEFT))      move(mOrientation.left(), speed);
        if (window.isKeyHeld(Key::KEY_RIGHT))     move(mOrientation.right(), speed);
        if (window.isKeyHeld(Key::KEY_PAGE_UP))   move(mOrientation.up(), speed);
        if (window.isKeyHeld(Key::KEY_PAGE_DOWN)) move(mOrientation.down(), speed);
    }

    vec3 left()            const                                   { return mOrientation.left(); }
    vec3 right()           const                                   { return mOrientation.right(); }
    vec3 up()              const                                   { return mOrientation.up(); }
    vec3 down()            const                                   { return mOrientation.down(); }
    vec3 front()           const                                   { return mOrientation.front(); }
    vec3 back()            const                                   { return mOrientation.back(); }

    vec3 position()        const                                   { return mPosition; }
    quat orientation()     const                                   { return mOrientation; }

    void move(const vec3 &direction, float amount)
    {
        mPosition += amount * direction;
    }

private:
    mat4 mProjectionMatrix;
    vec3 mPosition;
    quat mOrientation;
};

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
        uniform sampler2D quadTexture;
        void main() 
        {                         
            color = texture(quadTexture, fragTexCoord);
        }
    );

public:
    MeshRenderer(const Window &window, const Camera &camera)
    {
        shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
        texture = shader.getUniform<sampler2D>("quadTexture");
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

    void render(const Camera &camera, const Mesh &mesh) const
    {
        glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        texture.set(mesh.diffuseTexture().bind(GL_TEXTURE0));
        modelMatrix.set(mesh.modelMatrix());
        viewMatrix.set(camera.viewMatrix());
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
    auto window   = Window(1920, 1080, "Camera Sample");
    auto loop     = GameLoop(window, 60);
    auto camera   = Camera(43_deg, window.aspectRatio(), 0.001f, 1000.0f);
    auto renderer = MeshRenderer(window, camera);
    auto mesh     = Mesh("res/mesh/teapot.obj");
    
    mesh.scale(0.05f);
    mesh.moveBy(0,-2,0);

    camera.move(camera.back(), 10);
    renderer.bind(mesh);

    loop.run([&]() 
    {   
        camera.update(window);
        fprintln("camera pos %1% / front = %2% / orientation = %3%", camera.position(), camera.front(), camera.orientation());
        renderer.render(camera, mesh);
        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });

    return EXIT_SUCCESS;
}