#include <ray/entities/Cube.hpp>
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

class CubeMap 
{
public:
    CubeMap(const std::vector<std::string> &faces) { load(faces); }

    samplerCube bind(GLuint slotIndex=GL_TEXTURE0) const
    {
        glActiveTexture(slotIndex);
        glBindTexture(GL_TEXTURE_CUBE_MAP, mHandle);
        return samplerCube{GL_TEXTURE0-slotIndex};
    }

    void setSwizzle(GLint r, GLint g, GLint b, GLint a) const
    {
        bind();
        GLint swizzleMask[] = { r, g, b, a };
        gl(TexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
    }

    void loadFace(int target, const std::string filename)
    {
        auto bitmap = Bitmap(filename);
        switch(bitmap.depth())
        {
            case 3:
                gl(TexImage2D(target, 0, GL_RGB, bitmap.width(), bitmap.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.pixels()));
                setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ONE);
                break;
            case 4:   
                gl(TexImage2D(target, 0, GL_RGBA, bitmap.width(), bitmap.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.pixels()));
                setSwizzle(GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA);
                break;
            default:
                panic("unexpected number of channels '%d'", bitmap.depth());
        }
    }

    void load(const std::vector<std::string> &faces)
    {
        bind();
    
        loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, faces[0]);
        loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, faces[1]);
        loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, faces[2]);
        loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, faces[3]);
        loadFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, faces[4]);
        loadFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, faces[5]);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

private:
    static void destroy(GLuint handle) { gl(DeleteTextures(1, &handle)); }
    static void create(GLuint &handle) { gl(GenTextures(1, &handle)); }
    Handle<create, destroy> mHandle;
};

class Skybox : public Cube
{
public:
    Skybox(const std::vector<std::string> &faces) : mCubeMap(faces) {}
    const CubeMap &cubeMap() const { return mCubeMap; }
private:
    CubeMap mCubeMap;
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
        uniform samplerCube cubeMap;
        void main()
        {    
            FragColor = texture(cubeMap, TexCoords);
        }
    );

public:
    SkyboxRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER) 
    {
        projection = mShader.getUniform<mat4>("projection");
        view       = mShader.getUniform<mat4>("view");
        cubeMap    = mShader.getUniform<samplerCube>("cubeMap");
        position   = mShader.getAttribute<vec3>("vertPosition");
    }

    void bind(const Skybox &skybox)
    {
        skybox.bindPosition(position);
    }

    void render(const Camera &camera, const Skybox &skybox)
    {
        mShader.start();
        glEnable(GL_DEPTH_TEST);        
        glDepthFunc(GL_LEQUAL);
        view.set(stripTranslation(camera.viewMatrix()));
        projection.set(camera.projectionMatrix());
        cubeMap.set(skybox.cubeMap().bind(GL_TEXTURE0));
        skybox.draw();
        glDepthFunc(GL_LESS);
        glDisable(GL_DEPTH_TEST);        
        mShader.stop();
    }

private:
    ShaderProgram mShader;
    Uniform<samplerCube> cubeMap;
    Uniform<mat4> projection, view;
    Attribute<vec3> position;    
};

int main()
{
    auto window         = Window(1920, 1080, "Skybox");
    auto loop           = GameLoop(window, 60);
    auto camera         = Camera(45_deg, window.aspectRatio(), 0.001f, 1000.0f);
    auto skyboxRenderer = SkyboxRenderer();
    auto skybox         = Skybox({
        "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/back.jpg",
        "res/images/skybox/front.jpg",
    });

    skyboxRenderer.bind(skybox);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    loop.run([&]() 
    {
        camera.update(window, loop.dt().count());
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skyboxRenderer.render(camera, skybox);

        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });
        
    return 0;
}
