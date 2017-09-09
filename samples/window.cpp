#include <ray/platform/Window.hpp>
#include <cstdlib>

using namespace ray::platform;

int main()
{
    auto window = Window(1920, 1080, "Window Sample");

    gl(ClearColor(0.0f, 0.2f, 0.2f, 0.0f));

    while (!window.shouldClose())
    {
        gl(Clear(GL_COLOR_BUFFER_BIT));
        window.pollEvents();
        window.swapBuffers();
    }

    return EXIT_SUCCESS;
}