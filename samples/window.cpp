#include <ray/glfw/glfw3.hpp>
#include <iostream>
#include <cstdlib>

using namespace ray::glfw;

int main()
{
    auto window = Window(1920, 1080, "Window Sample");
    window.makeContextCurrent();
    window.loadGLExtensions();    
    window.swapInterval(0);

    glClearColor(0.0f, 0.2f, 0.2f, 0.0f);    

    while (!window.shouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}