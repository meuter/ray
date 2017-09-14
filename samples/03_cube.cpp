#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/components/Transformable.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::components;

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
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  0.0f, 1.0f,

            // Back face
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f,  0.0f, -1.0f,

            // Left face
            -0.5f,  0.5f, -0.5f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,    1.0f, 0.0f,     1.0f,  0.0f,  0.0f,

            // Right face 
            0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, +0.5f,     1.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, +0.5f,     0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, +0.5f,     0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,     0.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,     1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,

            // Bottom face
            -0.5f, -0.5f,  0.5f,    0.0f, 1.0f,     0.0f, 1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,    1.0f, 1.0f,     0.0f, 1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f, 1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f, 1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 1.0f,  0.0f,
            -0.5f, -0.5f, +0.5f,    0.0f, 1.0f,     0.0f, 1.0f,  0.0f,

            // Top face
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  -1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f,  -1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     0.0f,  -1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f,  -1.0f,  0.0f
        });        
        moveTo(0,0,-3);
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

        projectionMatrix = projection(43_deg, window.aspectRatio(), 0.01f, 1000.0f);        
    }

    void bind(const Cube &cube)
    {
        cube.bindPosition(shader.getAttribute<vec3>("vertPosition"));
        cube.bindTexCoord(shader.getAttribute<vec2>("vertTexCoord"));
    }
    
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
    auto window   = Window(1920, 1080, "Cube Sample");
    auto renderer = CubeRenderer(window);
    auto cube     = Cube("res/images/awesomeface.png");

    panicif(180_deg != PI, "oops");

    renderer.bind(cube);

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