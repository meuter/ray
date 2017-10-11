// #include <ray/components/Movable.hpp>
// #include <ray/components/Orientable.hpp>
#include <ray/math/Transform.hpp>
// #include <ray/entities/TransformableMesh.hpp>
#include <ray/platform/Window.hpp>
#include <ray/platform/GameLoop.hpp>
#include <ray/platform/Print.hpp>
// #include <ray/platform/FileSystem.hpp>
#include <ray/gl/VertexArray.hpp>
#include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
// #include <cstdlib>

using namespace ray;
using namespace platform;
using namespace math;
using namespace gl;
using namespace assets;
// using namespace components;
// using namespace entities;

#include <iostream>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
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


class Camera
{
public:
    vec3 Position, Front, Up, Right, WorldUp;
    rad Yaw, Pitch, Zoom;
    float MovementSpeed;
    float MouseSensitivity;

    // Constructor with vectors
    Camera(const vec3 &position = vec3(0.0f, 0.0f, 0.0f), const vec3 &up = vec3(0.0f, 1.0f, 0.0f), rad yaw = YAW, rad pitch = PITCH) : Front(0.0f, 0.0f, -1.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, rad yaw, rad pitch) : Front(0.0f, 0.0f, -1.0f), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = vec3(posX, posY, posZ);
        WorldUp = vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    mat4 GetViewMatrix()
    {
        return lookAt(Position, Position + Front, Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        println("deltaTime =", deltaTime);
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front*velocity;
        if (direction == BACKWARD)
            Position -= Front*velocity;
        if (direction == LEFT)
            Position -= Right*velocity;
        if (direction == RIGHT)
            Position +=  Right*velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // Update Front, Right and Up Vectors using the updated Eular angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors()
    {
        // Calculate the new Front vector
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
        layout (location = 0) in vec3 aPos;
        
        out vec3 TexCoords;
        
        uniform mat4 projection;
        uniform mat4 view;
        
        void main()
        {
            TexCoords = aPos;
            vec4 pos = projection * view * vec4(aPos, 1.0);
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
    }

    ShaderProgram mShader;
    Uniform<samplerCube> skybox;
    Uniform<mat4> projection, view;
};


class CubeRenderer 
{
    static constexpr auto VERTEX_SHADER = GLSL(330, 
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec2 aTexCoords;        
        out vec2 TexCoords;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main()
        {
            TexCoords = aTexCoords;    
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }  
    );
    
    static constexpr auto FRAGMENT_SHADER = GLSL(330,
        out vec4 FragColor;        
        in vec2 TexCoords;
        uniform sampler2D texture1;
        void main()
        {    
            FragColor = texture(texture1, TexCoords);
        }
    );

public:
    CubeRenderer() : mShader(VERTEX_SHADER, FRAGMENT_SHADER) {
        projection = mShader.getUniform<mat4>("projection");
        view = mShader.getUniform<mat4>("view");
        model = mShader.getUniform<mat4>("model");
        texture1 = mShader.getUniform<sampler2D>("texture1");
        position = mShader.getAttribute<vec3>("aPos");
        texCoord = mShader.getAttribute<vec2>("aTexCoords");
    }

    void render(const VertexArray &cubeVAO, const Texture &texture)
    {
        mShader.start();
    }

    ShaderProgram mShader;
    Uniform<sampler2D> texture1;
    Uniform<mat4> projection, view, model;
    Attribute<vec3> position;
    Attribute<vec2> texCoord;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(const Window &window, float deltaTime);
unsigned int loadCubemap(std::vector<std::string> faces);

// camera
Camera camera(vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)1920 / 2.0;
float lastY = (float)1082 / 2.0;
bool firstMouse = true;

int main()
{
    auto window = Window(1920, 1080, "Skybox");
    auto loop   = GameLoop(window, 60);

    window.setCursorPosCallback(mouse_callback);
    window.setScrollCallback(scroll_callback);

    // tell GLFW to capture our mouse
    window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    CubeRenderer cubeRenderer;
    SkyboxRenderer skyboxRenderer;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    auto skyboxVBO = VertexBuffer<f32,3>{
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
    };

    // cube VAO
    auto cubeVAO = Cube("res/images/marble.jpg");
    cubeVAO.bindPosition(cubeRenderer.position);
    cubeVAO.bindTexCoord(cubeRenderer.texCoord);

    // skybox VAO
    auto skyboxVAO = VertexArray();
    skyboxVAO.bindAttribute(Attribute<vec3>(0), skyboxVBO);

    std::vector<std::string> faces
    {
        "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/back.jpg",
        "res/images/skybox/front.jpg",
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // cubeRenderer configuration
    // --------------------
    cubeRenderer.mShader.start();
    cubeRenderer.texture1 = sampler2D{0};

    skyboxRenderer.mShader.start();
    skyboxRenderer.skybox = samplerCube{0};

    // render loop
    // -----------
    loop.run([&]() {

        processInput(window, loop.dt().count());
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        cubeRenderer.render(cubeVAO, cubeVAO.texture());
        mat4 view = camera.GetViewMatrix();
        auto projection = perspective(camera.Zoom, window.aspectRatio(), 0.1f, 100.0f);
        
        cubeRenderer.model = scaling(1.0f);
        cubeRenderer.view = view;
        cubeRenderer.projection = projection;

        // cubes
        cubeVAO.texture().bind(GL_TEXTURE0);
        cubeVAO.draw();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
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
        // skybox cube
        skyboxVAO.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVAO.unbind();
        glDepthFunc(GL_LESS); // set depth function back to default
    });
    

    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(const Window &window, float deltaTime)
{
    if (window.isKeyPressed(Key::KEY_ESCAPE))
        window.setShouldClose(true);


    if (window.isKeyHeld(Key::KEY_UP))
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (window.isKeyHeld(Key::KEY_DOWN))
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (window.isKeyHeld(Key::KEY_LEFT))
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (window.isKeyHeld(Key::KEY_RIGHT))
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
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


// #include <ray/components/Movable.hpp>
// #include <ray/components/Orientable.hpp>
// #include <ray/math/Transform.hpp>
// #include <ray/entities/TransformableMesh.hpp>
// #include <ray/platform/Window.hpp>
// #include <ray/platform/GameLoop.hpp>
// #include <ray/platform/FileSystem.hpp>
// #include <ray/gl/VertexArray.hpp>
// #include <ray/gl/ShaderProgram.hpp>
// #include <ray/gl/Texture.hpp>
// #include <cstdlib>

// using namespace ray::platform;
// using namespace ray::gl;
// using namespace ray::math;
// using namespace ray::assets;
// using namespace ray::components;
// using namespace ray::entities;


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



// struct SkyBox : public VertexArray
// {
//     SkyBox(const char *textureDirectory) 
//     {
//         // mTexture.load(textureFilename);
//         constexpr auto SIZE = 50;
//         vbo.load({
//             // Front face
//             -SIZE, -SIZE,  SIZE,
//              SIZE, -SIZE,  SIZE,
//              SIZE,  SIZE,  SIZE,
//              SIZE,  SIZE,  SIZE,
//             -SIZE,  SIZE,  SIZE,
//             -SIZE, -SIZE,  SIZE,

//             // Back face
//             -SIZE, -SIZE, -SIZE,
//              SIZE, -SIZE, -SIZE,
//              SIZE,  SIZE, -SIZE,
//              SIZE,  SIZE, -SIZE,
//             -SIZE,  SIZE, -SIZE,
//             -SIZE, -SIZE, -SIZE,

//             // Left face
//             -SIZE,  SIZE, -SIZE,
//             -SIZE,  SIZE,  SIZE,
//             -SIZE, -SIZE,  SIZE,
//             -SIZE, -SIZE,  SIZE,
//             -SIZE, -SIZE, -SIZE,
//             -SIZE,  SIZE, -SIZE,

//             // Right face 
//             SIZE,  SIZE, -SIZE,
//             SIZE,  SIZE, +SIZE,
//             SIZE, -SIZE, +SIZE,
//             SIZE, -SIZE, +SIZE,
//             SIZE, -SIZE, -SIZE,
//             SIZE,  SIZE, -SIZE,

//             // Bottom face
//             -SIZE, -SIZE,  SIZE,
//              SIZE, -SIZE,  SIZE,
//              SIZE, -SIZE, -SIZE,
//              SIZE, -SIZE, -SIZE,
//             -SIZE, -SIZE, -SIZE,
//             -SIZE, -SIZE, +SIZE,

//             // Top face
//             -SIZE,  SIZE, -SIZE, 
//              SIZE,  SIZE, -SIZE,
//              SIZE,  SIZE,  SIZE,
//              SIZE,  SIZE,  SIZE,
//             -SIZE,  SIZE,  SIZE,
//             -SIZE,  SIZE, -SIZE,
//         });        
//     }

//     void bindPosition(Attribute<vec3> position) const { bindAttributeAtOffset(0, position, vbo);  }

//     const CubeMap &cubeMap() const { return mCubeMap; }
    
//     void draw() const
//     {
//         bind();
//         glDrawArrays(GL_TRIANGLES, 0, vbo.vertexCount());
//         unbind();
//     }

// private:    
//     VertexBuffer<f32,3> vbo;
//     CubeMap mCubeMap;
// };

// class SkyboxRenderer
// {
//     static constexpr auto VERTEX_SHADER = GLSL(330, 
//         in  vec3 vertPosition;
//         out vec3 fragTexDirection;
//         uniform mat4 projectionMatrix;
//         uniform mat4 viewMatrix;
//         void main() 
//         { 
//             gl_Position = projectionMatrix * viewMatrix * vec4(vertPosition,1); 
//             fragTexDirection = gl_Position.xyz;
//         }
//     );
    
//     static constexpr auto FRAGMENT_SHADER = GLSL(330,
//         out vec3 fragTexDirection;
//         uniform samplerCube cubeMap;
//         out vec4 color;
//         void main() 
//         {                         
//             color = texture(cubeMap, fragTexDirection);
//         }
//     );

// public:
//     SkyboxRenderer(const Window &window, const Camera &camera)
//     {
//         shader.load(VERTEX_SHADER, FRAGMENT_SHADER);    
//         cubeMap = shader.getUniform<samplerCube>("cubeMap");
//         projectionMatrix = shader.getUniform<mat4>("projectionMatrix");  
//         viewMatrix = shader.getUniform<mat4>("viewMatrix");  

//         projectionMatrix = projection(43_deg, window.aspectRatio(), 0.01f, 1000.0f);        
//     }

//     void bind(const SkyBox &skybox)
//     {
//         skybox.bindPosition(shader.getAttribute<vec3>("vertPosition"));
//     }
    
//     void render(const Camera &camera, const SkyBox &skybox)
//     {
//         glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         shader.start();
//         glEnable(GL_DEPTH_TEST);
//         cubeMap.set(skybox.cubeMap().bind(GL_TEXTURE0));
//         skybox.draw();
//         glDisable(GL_DEPTH_TEST);
//         shader.stop();
//     }

// private:
//     ShaderProgram shader;
//     Uniform<samplerCube> cubeMap;
//     Uniform<mat4> projectionMatrix, viewMatrix;
// };

// int main()
// {    
//     auto window   = Window(1920, 1080, "Camera Sample");
//     auto loop     = GameLoop(window, 60);
//     auto camera   = Camera(43_deg, window.aspectRatio(), 0.001f, 1000.0f);
//     auto renderer = SkyboxRenderer(window, camera);
//     auto skybox   = SkyBox("res/images/skybox");
    
//     camera.move(camera.back(), 10);
//     renderer.bind(skybox);

//     loop.run([&]() 
//     {   
//          camera.update(window);
//         renderer.render(camera, skybox);
//         if (loop.frameCount()%60 == 0)
//             fprintln("average frame time = %1%msec", 1000*loop.averageFrameTime().count());
//     });

//     return EXIT_SUCCESS;
// }