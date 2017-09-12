#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/FileSystem.hpp>
#include <cstdlib>

#include <tiny_obj_loader.h>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;

struct TimedBlock
{
    TimedBlock(const char *description, const char *file, int line, const char *function) : mDescription(description), mFile(file), mLine(line), mFunction(function) {}
    ~TimedBlock() 
    {
        fprintln("%s:%d:%s: %fms elapsed for %s", mFile, mLine, mFunction, 1000*mStopwatch.lap().count(), mDescription);
    }
    const char *mFile, *mFunction, *mDescription;
    int mLine;
    Stopwatch mStopwatch;
};

#define TIMED(x) auto __timedBlock__ ## __COUNTER__ = TimedBlock(x, __FILE__, __LINE__, __FUNCTION__)

struct Mesh : public VertexArray, public Transformable
{
    Mesh(const std::string &objFilename, const std::string texFilename) 
    {
        TIMED("Constructor");
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        
        std::string error;
        std::string basepath = fs::parent(objFilename);
        bool success;

        success = tinyobj::LoadObj(&attrib, &shapes, &materials, &error, objFilename.c_str(), basepath.c_str());
        panicif(!success, "could not load '%s': %s", objFilename, error);
        
        int totalIndices = 0;
        for (auto &shape: shapes)
            totalIndices += shape.mesh.indices.size();

        vbo.reserve(5*totalIndices);
        auto mapped = vbo.map(GL_WRITE_ONLY);
        for (auto &shape: shapes)
        {
            auto &indices = shape.mesh.indices;
            for (auto i = 0u; i < indices.size(); i+=3)                
            {
                for (auto j = 0; j < 3; ++j)
                {
                    (*mapped++) = attrib.vertices[3*indices[i+j].vertex_index+0];
                    (*mapped++) = attrib.vertices[3*indices[i+j].vertex_index+1];
                    (*mapped++) = attrib.vertices[3*indices[i+j].vertex_index+2];
                    (*mapped++) = attrib.texcoords[2*indices[i+j].texcoord_index+0];
                    (*mapped++) = attrib.texcoords[2*indices[i+j].texcoord_index+1];
                }
            }
        }
        vbo.unmap();

        // TODO(cme): load texture from mtl file
        mTexture.load(texFilename);
        scale(0.02f);
        moveTo(0,-0.7f,+3);
        rotate(vec3(1,0,0), 10_deg);
    }

    void bindPosition(Attribute<vec3> position) const { bindAttributeAtOffset(0, position, vbo);  }
    void bindTexCoord(Attribute<vec2> texCoord) const { bindAttributeAtOffset(3, texCoord, vbo);  }
    const Texture &texture() const { return mTexture; }
    
    void draw() const
    {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, vbo.vertexCount());
        unbind();
    }

    void update() 
    {
        rotate(vec3(0,1,0), 2_deg);
    }

private:    
    VertexBuffer<f32,5> vbo;
    Texture mTexture;    
};

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

    
    void render(const Mesh &mesh)
    {
        glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        texture.set(mesh.texture().bind(GL_TEXTURE0));
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
    auto mesh     = Mesh("res/mesh/teapot.obj", "res/images/grid.png");

    mesh.bindPosition(renderer.position());
    mesh.bindTexCoord(renderer.texCoord());

    loop.run([&]() 
    {
        renderer.render(mesh);
        mesh.update();
        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });

    return EXIT_SUCCESS;
}