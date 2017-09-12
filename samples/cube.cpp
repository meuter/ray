#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;

struct Cube : public VertexArray, public Transformable
{
    Cube(const char *textureFilename) 
    {
        mTexture.load(textureFilename);
        vbo.load({
            ////////////////////////////////////////////////////////////
            // Position             TexCoord        Normal    
            ////////////////////////////////////////////////////////////
            // Front face
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, 1.0f,

            // Back face
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,
             0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,

            // Left face
            -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,

            // Right face 
            0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,     1.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,     0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,     0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,     0.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,     1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,

            // Bottom face
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f,  0.0f,

            // Top face
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  -1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  -1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  -1.0f,  0.0f
        });

        moveTo(0,0,+3);
    }

    void bindPosition(Attribute<vec3> position) const { bindAttributeAtOffset(0, position, vbo);  }
    void bindTexCoord(Attribute<vec2> texCoord) const { bindAttributeAtOffset(3, texCoord, vbo);  }
    void bindNormal(Attribute<vec3> normal)     const { bindAttributeAtOffset(5, normal, vbo);    }
    const Texture &texture() const { return mTexture; }
    
    void draw() const
    {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, vbo.vertexCount());
        unbind();
    }

    void update() 
    {
        rotate(vec3(1,0,0), 2_deg);
    }

private:    
    VertexBuffer<f32,8> vbo;
    Texture mTexture;
};

class CubeRenderer
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
    CubeRenderer(const Window &window)
    {
        shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
        texture = shader.getUniform<sampler2D>("quadTexture");
        modelMatrix = shader.getUniform<mat4>("modelMatrix");
        projectionMatrix = shader.getUniform<mat4>("projectionMatrix");    

        projectionMatrix = getProjectionMatrix(window, 43.0f, 0.001f, 1000.0f);
    }

    mat4 getProjectionMatrix(const Window &window, float fov, float zNear, float zFar) const
    {
        ivec2 windowSize;
        window.getSize(windowSize.w, windowSize.h);
        float aspectRatio = (float)windowSize.w / windowSize.h;
        float zRange = zNear - zFar;
        float yScale = -1.0f / tan(fov/2.0f);
        float xScale = -1 * yScale / aspectRatio;
        float zScale = -1 * (zNear+zFar)/zRange;
        float zTranslate = 2.0f*zFar*zNear/zRange;
    
        return mat4{
            xScale, 0.0f,   0.0f,    0.0f,
            0.0f,   yScale, 0.0f,    0.0f,
            0.0f,   0.0f,   zScale,  zTranslate,
            0.0f,   0.0f,   1.0f,    0.0f
        };
    }

    Attribute<vec3> position() { return shader.getAttribute<vec3>("vertPosition"); }
    Attribute<vec2> texCoord() { return shader.getAttribute<vec2>("vertTexCoord"); }

    
    void render(const Cube &cube)
    {
        glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        texture.set(cube.texture().bind(GL_TEXTURE0));
        modelMatrix.set(cube.modelMatrix());
        cube.draw();
        glDisable(GL_DEPTH_TEST);
    }

private:
    ShaderProgram shader;
    Uniform<sampler2D> texture;
    Uniform<mat4> modelMatrix, projectionMatrix, viewMatrix;
};

int main()
{    
    auto window   = Window(1920, 1080, "Texture Sample");
    auto renderer = CubeRenderer(window);
    auto cube     = Cube("res/images/awesomeface.png");

    cube.bindPosition(renderer.position());
    cube.bindTexCoord(renderer.texCoord());

    window.swapInterval(1);

    while (!window.shouldClose())
    {
        renderer.render(cube);
        cube.update();

        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}