#include <ray/entities/Camera.hpp>
#include <ray/components/Movable.hpp>
#include <ray/components/Orientable.hpp>
#include <ray/math/Transform.hpp>
#include <ray/platform/Window.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/Print.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>

using namespace ray;
using namespace platform;
using namespace math;
using namespace gl;
using namespace assets;
using namespace components;
using namespace entities;

class Cube : public VertexArray 
{
public:
    Cube(const std::string &textureFilename) : mTexture(textureFilename)
    {
        mVertexBuffer.load({
            // positions          // texture Coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        });
    }

    void bindPosition(Attribute<vec3> position) const { bindAttributeAtOffset(0, position, mVertexBuffer);  }
    void bindTexCoord(Attribute<vec2> texCoord) const { bindAttributeAtOffset(3, texCoord, mVertexBuffer);  }

    const Texture &texture() const { return mTexture; }

    void draw() const
    {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer.vertexCount());
        unbind();
    }

private:
    VertexBuffer<f32,5> mVertexBuffer;
    Texture mTexture;
};

class Skybox : public VertexArray
{
public:
    Skybox() 
    {
        mVertexBuffer.load({
            // positions          
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
    
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
    
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
    
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
    
            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
    
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        });
    }

    void draw() const
    {
        bind();
        glDrawArrays(GL_TRIANGLES, 0, mVertexBuffer.vertexCount());
        unbind();
    }

    void bindPosition(Attribute<vec3> position) const { bindAttributeAtOffset(0, position, mVertexBuffer);  }

private:
    VertexBuffer<f32,3> mVertexBuffer;
};


class SkyboxRenderer 
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in vec3 vertPosition;
        out vec3 TexCoords;
        uniform mat4 projection;
        uniform mat4 view;
        
        void main()
        {
            TexCoords = vertPosition;
            vec4 pos = projection * view * vec4(vertPosition, 1.0);
            gl_Position = pos.xyww;
        }  
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        out vec4 FragColor;
        in vec3 TexCoords;
        uniform samplerCube skybox;
        void main()
        {    
            FragColor = texture(skybox, TexCoords);
        }
    );

public:
    SkyboxRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER) {
        projection = mShader.getUniform<mat4>("projection");
        view = mShader.getUniform<mat4>("view");
        skybox = mShader.getUniform<samplerCube>("skybox");
        position = mShader.getAttribute<vec3>("vertPosition");
    }

    ShaderProgram mShader;
    Uniform<samplerCube> skybox;
    Uniform<mat4> projection, view;
    Attribute<vec3> position;
};


class CubeRenderer 
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in vec3 vertPosition;
        in vec2 vertTexCoord;        
        out vec2 fragTexCoord;
        uniform mat4 modelMatrix;
        uniform mat4 viewMatrix;
        uniform mat4 projection;
        void main()
        {
            fragTexCoord = vertTexCoord;    
            gl_Position = projection * viewMatrix * modelMatrix * vec4(vertPosition, 1.0);
        }  
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        out vec4 FragColor;        
        in vec2 fragTexCoord;
        uniform sampler2D diffuseTexture;
        void main()
        {    
            FragColor = texture(diffuseTexture, fragTexCoord);
        }
    );

public:
    CubeRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER) 
    {
        projection = mShader.getUniform<mat4>("projection");
        viewMatrix = mShader.getUniform<mat4>("viewMatrix");
        modelMatrix = mShader.getUniform<mat4>("modelMatrix");
        texture = mShader.getUniform<sampler2D>("diffuseTexture");
        position = mShader.getAttribute<vec3>("vertPosition");
        texCoord = mShader.getAttribute<vec2>("vertTexCoord");

        modelMatrix.set(scaling(1.0f));
    }

    void render(const Cube &cube)
    {
        mShader.start();
    }

    ShaderProgram mShader;
    Uniform<sampler2D> texture;
    Uniform<mat4> projection, viewMatrix, modelMatrix;
    Attribute<vec3> position;
    Attribute<vec2> texCoord;
};

// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
// {
//     camera.ProcessMouseScroll(yoffset);
// }

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        auto bitmap = Bitmap(faces[i]);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels());
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int main()
{
    auto window         = Window(1920, 1080, "Skybox");
    auto loop           = GameLoop(window, 60);
    auto cubeRenderer   = CubeRenderer();
    auto skyboxRenderer = SkyboxRenderer();
    auto camera         = Camera(45_deg, window.aspectRatio(), 0.001f, 1000.0f);

    // window.setCursorPosCallback(mouse_callback);
    // window.setScrollCallback(scroll_callback);
    // window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    camera.moveTo(0.0f, 0.0f, 3.0f);
    camera.rotate(vec3(0,1,0), 180_deg);


    glEnable(GL_DEPTH_TEST);

    auto cube = Cube("res/images/marble.jpg");
    cube.bindPosition(cubeRenderer.position);
    cube.bindTexCoord(cubeRenderer.texCoord);

    auto skyboxVAO = Skybox();
    skyboxVAO.bindPosition(skyboxRenderer.position);

    std::vector<std::string> faces
    {
        "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/back.jpg",
        "res/images/skybox/front.jpg",
    };
    auto cubemapTexture = loadCubemap(faces);

    skyboxRenderer.mShader.start();
    skyboxRenderer.skybox = samplerCube{0};

    loop.run([&]() {

        camera.update(window, loop.dt().count());

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = camera.viewMatrix();

        cubeRenderer.render(cube); 
        cubeRenderer.texture = cube.texture().bind(GL_TEXTURE0) ;      
        cubeRenderer.viewMatrix = view;
        cubeRenderer.projection = camera.projectionMatrix();
        cube.draw();

        glDepthFunc(GL_LEQUAL);
        skyboxRenderer.mShader.start();
        view(0,3) = 0;
        view(1,3) = 0;
        view(2,3) = 0;
        view(4,3) = 1;
        view(3,0) = 0;
        view(3,1) = 0;
        view(3,2) = 0;
        skyboxRenderer.view = view;
        skyboxRenderer.projection = camera.projectionMatrix();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skyboxVAO.draw();
        glDepthFunc(GL_LESS);

        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });
    
    return 0;
}

// class CubeMap
// {
// public:
//     CubeMap() 
//     {
//         glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
//         {
//             gl(TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//             gl(TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//             gl(TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
//             gl(TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
//             gl(TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)); 
//             loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, "res/images/skybox/right.png");
//             loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "res/images/skybox/left.png");
//             loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "res/images/skybox/top.png");
//             loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "res/images/skybox/bottom.png");
//             loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "res/images/skybox/back.png");
//             loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "res/images/skybox/front.png");
//         }
//         glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
//     }

//     samplerCube bind(GLuint textureSlot=GL_TEXTURE0) const
//     {
//         gl(ActiveTexture(textureSlot));
//         gl(BindTexture(GL_TEXTURE_CUBE_MAP, mHandle));
//         return samplerCube{GL_TEXTURE0-textureSlot};
//     }   
    
//     void setSwizzle(GLint r, GLint g, GLint b, GLint a) const
//     {
//         bind();
//         GLint swizzleMask[] = { r, g, b, a };
//         gl(TexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
//     }

//     void loadFace(int target, const std::string filename)
//     {
//         bind();
//         auto bitmap = Bitmap(filename);
//         switch(bitmap.depth())
//         {
//             case 3:
//                 gl(TexImage2D(target, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels()));
//                 setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ONE);
//                 break;
//             case 4:   
//                 gl(TexImage2D(target, 0, GL_RGBA, bitmap.width(), bitmap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels()));
//                 setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA);
//                 break;
//             default:
//                 panic("unexpected number of channels '%d'", bitmap.depth());
//         }
//     }

// private:
//     static void destroy(GLuint handle) { gl(DeleteTextures(1, &handle)); }
//     static void create(GLuint &handle) { gl(GenTextures(1, &handle)); }
//     Handle<create, destroy> mHandle;
// };
