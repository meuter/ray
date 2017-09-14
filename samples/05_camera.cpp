#include <ray/components/Movable.hpp>
#include <ray/components/Orientable.hpp>
#include <ray/math/Transform.hpp>
#include <ray/entities/Mesh.hpp>
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
using namespace ray::components;


class Camera : public Movable, public Orientable
{
public:
    Camera(rad fovy, float aspect, float n, float f) : Orientable(0,0,-1), mProjectionMatrix(projection(fovy, aspect, n, f)) {}

    mat4 projectionMatrix() const  { return mProjectionMatrix; }

    mat4 viewMatrix() const 
    {
        // NOTE(cme): HACK!! This does what I want, by I cannot understand why from a math pov
        auto displacement = position();
        displacement.y = -displacement.y;
        // should be displacement = -mPosition;
        // END HACK 

        auto inverseTranslation = translation(displacement);
        auto inverseRotation = rotation(conjugate(orientation()));

        return  inverseRotation * inverseTranslation;
    }

    void update(const Window &window)
    {
        moveUsingKeyboard(window);        
        lookUsingMouse(window);
    }

    void lookUsingMouse(const Window &window, float sensitivity=0.001f)
    {
        static dvec2 lastCursorPos;
        if (window.isMouseButtonReleased(MouseButton::BUTTON_LEFT))
        {
            window.showMouseCursor();		
        }
        else if (window.isMouseButtonPressed(MouseButton::BUTTON_LEFT))
        {
            window.disableMouseCursor();
            window.getCursorPosition(lastCursorPos.x, lastCursorPos.y);
        }
        else if (window.isMouseButtonHeld(MouseButton::BUTTON_LEFT))
        {
            auto newPos = dvec2();
            window.getCursorPosition(newPos.x, newPos.y);
            auto dpos = lastCursorPos - newPos;
            lastCursorPos = newPos;
            if (dpos.x != 0) rotate(vec3(0,1,0), sensitivity * dpos.x);
            if (dpos.y != 0) rotate(left(), sensitivity * dpos.y);
        }
    }

    void moveUsingKeyboard(const Window &window, float speed=0.1f)
    {
        if (window.isKeyHeld(Key::KEY_UP))        move(front(), speed);
        if (window.isKeyHeld(Key::KEY_DOWN))      move(back(), speed);
        if (window.isKeyHeld(Key::KEY_LEFT))      move(left(), speed);
        if (window.isKeyHeld(Key::KEY_RIGHT))     move(right(), speed);
        if (window.isKeyHeld(Key::KEY_PAGE_UP))   move(up(), speed);
        if (window.isKeyHeld(Key::KEY_PAGE_DOWN)) move(down(), speed);
    }

private:
    mat4 mProjectionMatrix;
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
    mesh.move(vec3(0,-1,0), 2);

    camera.move(camera.back(), 10);
    renderer.bind(mesh);

    loop.run([&]() 
    {   
        camera.update(window);
        renderer.render(camera, mesh);
        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });

    return EXIT_SUCCESS;
}