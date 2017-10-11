// #include <ray/components/Movable.hpp>
// #include <ray/components/Orientable.hpp>
// #include <ray/math/Transform.hpp>
// #include <ray/entities/TransformableMesh.hpp>
#include <ray/platform/Window.hpp>
// #include <ray/platform/GameLoop.hpp>
// #include <ray/platform/FileSystem.hpp>
#include <ray/gl/VertexArray.hpp>
// #include <ray/gl/ShaderProgram.hpp>
#include <ray/gl/Texture.hpp>
// #include <cstdlib>

using namespace ray::platform;
using namespace ray::gl;
// using namespace ray::math;
// using namespace ray::assets;
// using namespace ray::components;
// using namespace ray::entities;




#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();			
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char * fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW        = -90.0f;
const float PITCH      =  0.0f;
const float SPEED      =  2.5f;
const float SENSITIVTY =  0.1f;
const float ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Eular Angles
    float Yaw;
    float Pitch;
    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
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
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // Also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(std::vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    auto rayWindow = Window(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL");
    auto window = rayWindow.mHandle;

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("res/shaders/6.1.cubemaps.vs", "res/shaders/6.1.cubemaps.fs");
    Shader skyboxShader("res/shaders/6.1.skybox.vs", "res/shaders/6.1.skybox.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
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
    };
    float skyboxVertices[] = {
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
    auto cubeVAO = VertexArray();
    unsigned int cubeVBO;
    glGenBuffers(1, &cubeVBO);
    cubeVAO.bind();
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    cubeVAO.unbind();

    // skybox VAO
    auto skyboxVAO = VertexArray();
    unsigned int skyboxVBO;
    glGenBuffers(1, &skyboxVBO);
    skyboxVAO.bind();
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    skyboxVAO.unbind();

    // load textures
    // -------------    
    auto cubeTexture = Texture("res/images/marble.jpg");

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

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    // render loop
    // -----------
    while (!rayWindow.shouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw scene as normal
        shader.use();
        glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        cubeVAO.bind();
        cubeTexture.bind(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cubeVAO.unbind();

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        skyboxVAO.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        skyboxVAO.unbind();
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        rayWindow.swapBuffers();
        rayWindow.pollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
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
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
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