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

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const rad YAW        = -90_deg;
const rad PITCH      =  0_deg;
const rad ZOOM       =  45_deg;
const rad SENSITIVTY =  0.1_deg;
const float SPEED      =  2.5f;

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


class Camera : public Movable, public Orientable
{
public:
    vec3 Front, Up, Right, WorldUp;
    rad Yaw, Pitch, Zoom;
    float MovementSpeed;
    float MouseSensitivity;

    Camera(const vec3 &position = vec3(0.0f, 0.0f, 0.0f), const vec3 &up = vec3(0.0f, 1.0f, 0.0f), rad yaw = YAW, rad pitch = PITCH) : Front(0.0f, 0.0f, -1.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        moveTo(position);
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    mat4 GetViewMatrix()
    {
        return lookAt(position(), position() + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float amount = MovementSpeed * deltaTime;
        if (direction == FORWARD)  move(Front,  amount);
        if (direction == BACKWARD) move(Front, -amount);
        if (direction == LEFT)     move(Right, -amount);
        if (direction == RIGHT)    move(Right,  amount);
    }

    void ProcessMouseMovement(float xoffset, float yoffset)
    {
        Yaw   = Yaw+xoffset*MouseSensitivity;
        Pitch = clamp(-89_deg, Pitch+yoffset*MouseSensitivity, 89_deg);
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom = clamp(1_deg, Zoom - yoffset*PI_OVER_180, 63_deg);
    }

private:
    void updateCameraVectors()
    {
        vec3 front;
        front.x = cos(Yaw) * cos(Pitch);
        front.y = sin(Pitch);
        front.z = sin(Yaw) * cos(Pitch);
        Front = normalize(front);
        Right = normalize(cross(Front, WorldUp)); 
        Up    = normalize(cross(Right, Front));
    }
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

Camera camera(vec3(0.0f, 0.0f, 3.0f));

void processInput(const Window &window, float deltaTime)
{
    if (window.isKeyHeld(Key::KEY_UP))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (window.isKeyHeld(Key::KEY_DOWN))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (window.isKeyHeld(Key::KEY_LEFT))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (window.isKeyHeld(Key::KEY_RIGHT))
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = (float)1920 / 2.0;
    static float lastY = (float)1082 / 2.0;
    static bool firstMouse = true;
    
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

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

    window.setCursorPosCallback(mouse_callback);
    window.setScrollCallback(scroll_callback);
    window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

        processInput(window, loop.dt().count());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = camera.GetViewMatrix();
        auto projection = perspective(camera.Zoom, window.aspectRatio(), 0.1f, 100.0f);

        cubeRenderer.render(cube); 
        cubeRenderer.texture = cube.texture().bind(GL_TEXTURE0) ;      
        cubeRenderer.viewMatrix = view;
        cubeRenderer.projection = projection;
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
        skyboxRenderer.projection = projection;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skyboxVAO.draw();
        glDepthFunc(GL_LESS);

        if (loop.frameCount()%60 == 0)
            fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
    });
    
    return 0;
}

// class Camera : public Movable, public Orientable
// {
// public:
//     Camera(rad fovy, float aspect, float n, float f) : Orientable(0,0,-1), mProjectionMatrix(projection(fovy, aspect, n, f)) {}

//     mat4 projectionMatrix() const  { return mProjectionMatrix; }

//     mat4 viewMatrix() const 
//     {
//         // NOTE(cme): HACK!! This does what I want, by I cannot understand why from a math pov
//         auto displacement = position();
//         displacement.y = -displacement.y;
//         // should be displacement = -mPosition;
//         // END HACK 

//         auto inverseTranslation = translation(displacement);
//         auto inverseRotation = rotation(conjugate(orientation()));

//         return  inverseRotation * inverseTranslation;
//     }

//     void update(const Window &window)
//     {
//         moveUsingKeyboard(window);        
//         lookUsingMouse(window);
//     }

//     void lookUsingMouse(const Window &window, float sensitivity=0.001f)
//     {
//         static dvec2 lastCursorPos;
//         if (window.isMouseButtonReleased(MouseButton::BUTTON_LEFT))
//         {
//             window.showMouseCursor();		
//         }
//         else if (window.isMouseButtonPressed(MouseButton::BUTTON_LEFT))
//         {
//             window.disableMouseCursor();
//             window.getCursorPosition(lastCursorPos.x, lastCursorPos.y);
//         }
//         else if (window.isMouseButtonHeld(MouseButton::BUTTON_LEFT))
//         {
//             auto newPos = dvec2();
//             window.getCursorPosition(newPos.x, newPos.y);
//             auto dpos = lastCursorPos - newPos;
//             lastCursorPos = newPos;
//             if (dpos.x != 0) rotate(vec3(0,1,0), sensitivity * dpos.x);
//             if (dpos.y != 0) rotate(left(), sensitivity * dpos.y);
//         }
//     }

//     void moveUsingKeyboard(const Window &window, float speed=0.1f)
//     {
//         if (window.isKeyHeld(Key::KEY_UP))        move(front(), speed);
//         if (window.isKeyHeld(Key::KEY_DOWN))      move(back(), speed);
//         if (window.isKeyHeld(Key::KEY_LEFT))      move(left(), speed);
//         if (window.isKeyHeld(Key::KEY_RIGHT))     move(right(), speed);
//         if (window.isKeyHeld(Key::KEY_PAGE_UP))   move(up(), speed);
//         if (window.isKeyHeld(Key::KEY_PAGE_DOWN)) move(down(), speed);
//     }

// private:
//     mat4 mProjectionMatrix;
// };

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
