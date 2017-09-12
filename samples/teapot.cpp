#include <ray/platform/Window.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/platform/FileSystem.hpp>
#include <cstdlib>

#include <tiny_obj_loader.h>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;

struct Mesh : public VertexArray, public Transformable
{
    Mesh(const std::string &objFilename, const std::string texFilename) 
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        
        std::string error;
        std::string basepath = fs::parent(objFilename);
        bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &error, objFilename.c_str(), basepath.c_str());

        panicif(!success, "could not load '%s': %s", objFilename, error);
        
        auto &mesh = shapes[0].mesh;

        // TODO(cme): load directly in mapped VBO
        std::vector<float> vertexData;

        for (auto &shape: shapes)
        {
            auto &mesh = shape.mesh;
            for (auto i = 0u; i < mesh.indices.size(); i+=3)
            {
                auto index1 = mesh.indices[i+0];
                auto index2 = mesh.indices[i+1];
                auto index3 = mesh.indices[i+2];
    
                auto x1 = attrib.vertices[3*index1.vertex_index+0];
                auto y1 = attrib.vertices[3*index1.vertex_index+1];
                auto z1 = attrib.vertices[3*index1.vertex_index+2];
                auto u1 = attrib.texcoords[2*index1.texcoord_index+0];
                auto v1 = attrib.texcoords[2*index1.texcoord_index+1];
                vertexData.push_back(x1);
                vertexData.push_back(y1);
                vertexData.push_back(z1);
                vertexData.push_back(u1);
                vertexData.push_back(v1);
    
                auto x2 = attrib.vertices[3*index2.vertex_index+0];
                auto y2 = attrib.vertices[3*index2.vertex_index+1];
                auto z2 = attrib.vertices[3*index2.vertex_index+2];
                auto u2 = attrib.texcoords[2*index2.texcoord_index+0];
                auto v2 = attrib.texcoords[2*index2.texcoord_index+1];
                vertexData.push_back(x2);
                vertexData.push_back(y2);
                vertexData.push_back(z2);
                vertexData.push_back(u2);
                vertexData.push_back(v2);
                
                auto x3 = attrib.vertices[3*index3.vertex_index+0];
                auto y3 = attrib.vertices[3*index3.vertex_index+1];
                auto z3 = attrib.vertices[3*index3.vertex_index+2];
                auto u3 = attrib.texcoords[2*index3.texcoord_index+0];
                auto v3 = attrib.texcoords[2*index3.texcoord_index+1];
                vertexData.push_back(x3);
                vertexData.push_back(y3);
                vertexData.push_back(z3);
                vertexData.push_back(u3);
                vertexData.push_back(v3);
            }
        }

        vbo.load(vertexData);

        // TODO(cme): load material
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
    auto renderer = MeshRenderer(window);
    auto mesh     = Mesh("res/mesh/teapot.obj", "res/images/grid.png");

    mesh.bindPosition(renderer.position());
    mesh.bindTexCoord(renderer.texCoord());

    window.swapInterval(1);

    while (!window.shouldClose())
    {
        renderer.render(mesh);
        mesh.update();

        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}