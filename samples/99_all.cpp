#include <ray/platform/Window.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/FileSystem.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
#include <ray/gl/CubeMap.hpp>
#include <ray/entities/TransformableMesh.hpp>
#include <ray/entities/Cube.hpp>
#include <ray/entities/Camera.hpp>
#include <ray/components/Movable.hpp>
#include <ray/assets/Font.hpp>
#include <ray/components/TextureAtlas.hpp>
#include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
using namespace ray::math;
using namespace ray::assets;
using namespace ray::components;
using namespace ray::entities;

class CachedFont : public Font
{
public:
    CachedFont(const std::string &filename, int lineHeight) : Font(filename, lineHeight), mGlyphAtlas(1) {}

    const rect2 &getGlyphTextureCoordinates(u16 index)
    {
        auto hit = mGlyphCache.find(index);
        if (hit != mGlyphCache.end()) return hit->second;
        return (mGlyphCache[index] = mGlyphAtlas.add(rasterizeGlyph(index)));
    }

    const Texture &glyphAtlas() const { return mGlyphAtlas; }

private:
    TextureAtlas mGlyphAtlas;    
    std::unordered_map<u16, rect2> mGlyphCache;    
};

class TextRenderer
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        in  vec2 vertPosition;
        in  vec2 vertTexCoord;
        out vec2 fragTexCoord;
        uniform mat4 transform;
        void main() 
        { 
            fragTexCoord = vertTexCoord;
            gl_Position = transform * vec4(vertPosition,0,1); 
        }
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        in  vec3 fragColor;
        in  vec2 fragTexCoord;
        out vec4 color;
        uniform sampler2D quadTexture;
        uniform vec4 textColor;
        void main() 
        {                         
            color = texture(quadTexture, fragTexCoord) * textColor;
        }
    );

    struct Vertex { vec2 xy, uv; };
    
public:
    static constexpr auto MAX_LETTERS = 1024;
    static constexpr auto N_VERTEX_PER_LETTERS = 4;
    static constexpr auto N_FLOATS_PER_VERTEX  = sizeof(Vertex)/sizeof(float);
    static constexpr auto N_FLOATS_PER_LETTER  = N_VERTEX_PER_LETTERS*N_FLOATS_PER_VERTEX;
    static constexpr auto N_INDICES_PER_LETTER = 6;

    TextRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER)
    {        
        mVertexBuffer.reserve(MAX_LETTERS * N_FLOATS_PER_LETTER, GL_STREAM_DRAW);
        mIndexBuffer.reserve(MAX_LETTERS * N_INDICES_PER_LETTER);        
        mQuads.bindAttributeAtOffset(0, mShader.getAttribute<vec2>("vertPosition"), mVertexBuffer);
        mQuads.bindAttributeAtOffset(2, mShader.getAttribute<vec2>("vertTexCoord"), mVertexBuffer);            
        mQuads.bindIndices(mIndexBuffer);
        mTextColor = mShader.getUniform<vec4>("textColor");
        mQuadsTexture = mShader.getUniform<sampler2D>("quadTexture");
        mTransform = mShader.getUniform<mat4>("transform");

        auto mappedIndices = mIndexBuffer.map(GL_WRITE_ONLY);
        auto index = 0;
        for (auto l = 0; l < MAX_LETTERS; l += N_INDICES_PER_LETTER)        
        {
            (*mappedIndices++) = index+0; 
            (*mappedIndices++) = index+1;
            (*mappedIndices++) = index+2; 
            (*mappedIndices++) = index+1; 
            (*mappedIndices++) = index+2;
            (*mappedIndices++) = index+3; 
            index += 4;
        }
        mIndexBuffer.unmap();
    }

    static ivec2 getViewport()
    {
        int viewPort[4];
        glGetIntegerv( GL_VIEWPORT, viewPort);
        return { viewPort[2], viewPort[3] };
    }

    vec2 renderText(const vec2 &pos, CachedFont &font, const Color &color, const std::string &u8Text)
    {
        auto cursor = pos;
        auto quadVertices = reinterpret_cast<Vertex*>(mVertexBuffer.map(GL_WRITE_ONLY));
        auto nLetters = 0;
        for (auto codepoint: u8Text)
        {
            if (codepoint == '\n') cursor = { pos.x, cursor.y+font.lineHeight() };

            auto index       = font.getGlyphIndex(codepoint);
            auto metrics     = font.getGlyphMetrics(index);    
            auto bb          = metrics.boundingBox();
            auto uv          = font.getGlyphTextureCoordinates(index);
            auto topLeft     = cursor + bb.min;
            auto bottomRight = cursor + bb.max;

            (*quadVertices++) = { {bottomRight.x, topLeft.y},     {uv.max.x, uv.min.y} };
            (*quadVertices++) = { {topLeft.x,     topLeft.y},     {uv.min.x, uv.min.y} };
            (*quadVertices++) = { {bottomRight.x, bottomRight.y}, {uv.max.x, uv.max.y} };
            (*quadVertices++) = { {topLeft.x,     bottomRight.y}, {uv.min.x, uv.max.y} };

            cursor.x += metrics.advance();
            nLetters += 1;
        }     

        mVertexBuffer.unmap();

        auto viewport = getViewport();
        mShader.start();
        mTransform.set({
            2.0f/viewport.w, 0.0f, 0.0f, -1.0f,
            0.0f, -2.0f/viewport.h, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });

        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    
        mQuadsTexture.set(font.glyphAtlas().bind(GL_TEXTURE0));
        mTextColor.set(color);
        mQuads.bind();
        gl(DrawElements(GL_TRIANGLES, N_INDICES_PER_LETTER * nLetters, GL_UNSIGNED_INT, 0));
        glDisable(GL_BLEND);

        mShader.stop();
        return cursor;
    }

private:
    ShaderProgram mShader;
    VertexArray mQuads;
    VertexBuffer<f32,4> mVertexBuffer;
    Uniform<vec4> mTextColor;
    Uniform<sampler2D> mQuadsTexture;
    Uniform<mat4> mTransform;
    ElementBuffer mIndexBuffer;
};






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
        uniform mat4 viewMatrix;
        uniform vec3 lightPosition;
        uniform vec3 cameraPosition;

        void main() 
        { 
            vec4 worldPosition = modelMatrix * vec4(vertPosition,1);
            gl_Position = projectionMatrix * viewMatrix * worldPosition;
            surfaceNormal = (modelMatrix * vec4(vertNormal, 0)).xyz;
            lightVector = lightPosition - worldPosition.xyz;
            cameraVector = cameraPosition - worldPosition.xyz; 
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
        (void)window;
        shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
        modelMatrix = shader.getUniform<mat4>("modelMatrix");
        projectionMatrix = shader.getUniform<mat4>("projectionMatrix");    
        viewMatrix = shader.getUniform<mat4>("viewMatrix");    
        modelColor = shader.getUniform<vec4>("modelColor");
        lightColor = shader.getUniform<vec4>("lightColor");
        lightPosition = shader.getUniform<vec3>("lightPosition");
        cameraPosition = shader.getUniform<vec3>("cameraPosition");
        reflectivity = shader.getUniform<float>("reflectivity");
        shineDamper = shader.getUniform<float>("shineDamper");
        shader.stop();
    }

    void bind(const Mesh &mesh) const
    {
        mesh.bindPosition(shader.getAttribute<vec3>("vertPosition"));
        mesh.bindNormal(shader.getAttribute<vec3>("vertNormal"));
    }

    void render(const TransformableMesh &mesh, const Material &material, const Light &light, const Camera &camera) const
    {
        shader.start();
        glEnable(GL_DEPTH_TEST);
        modelMatrix.set(mesh.modelMatrix());
        modelColor.set(material.color);
        reflectivity.set(material.reflectivity);
        shineDamper.set(material.shineDamper);
        lightColor.set(light.color);
        lightPosition.set(light.position());
        cameraPosition.set(camera.position());
        viewMatrix.set(camera.viewMatrix());
        projectionMatrix.set(camera.projectionMatrix());
        mesh.draw();
        glDisable(GL_DEPTH_TEST);
        shader.stop();
    }

private:
    ShaderProgram shader;
    Uniform<mat4> modelMatrix, projectionMatrix, viewMatrix;
    Uniform<vec4> modelColor, lightColor;
    Uniform<vec3> lightPosition, cameraPosition;
    Uniform<float> reflectivity, shineDamper;
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
    auto window   = Window(1920, 1080, "Lighting Sample");
    auto loop     = GameLoop(window, 60);
    auto renderer = MeshRenderer(window);
    auto mesh     = TransformableMesh("res/mesh/bunny.obj");
    auto material = Material{DARK_GRAY, 1.0f, 10.0f};
    auto light    = Light(vec3(2,2,5), YELLOW);
    auto small    = CachedFont("res/fonts/Roboto-Regular.ttf", 30);
    auto texter   = TextRenderer();
    auto camera   = Camera(43_deg, window.aspectRatio(), 0.001f, 1000.0f);
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
    
    mesh.moveTo(0,-0.8f,-3.5f);

    renderer.bind(mesh);

    camera.moveTo(0.0f, 0.0f, 3.0f);
    camera.rotate(vec3(0,1,0), 180_deg);

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    

    loop.run([&]() 
    {      
        camera.update(window, loop.dt().count());
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto fps = fmt("average frame time = %6.3fmsec", 1000*loop.averageFrameTime().count());
        
        skyboxRenderer.render(camera, skybox);
        texter.renderText(vec2(0,0), small, RED, fps);
        renderer.render(mesh, material, light, camera);
        mesh.rotate(vec3(0,1,0), 2_deg);
    });

    return EXIT_SUCCESS;
}