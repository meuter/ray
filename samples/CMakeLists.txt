
set(CMAKE_CXX_STANDARD 11)

include_directories(
    ../include/
    ../contrib/glfw/include
)

link_libraries(
    glfw
    glad
    ${OPENGL_gl_LIBRARY}    
)

add_executable(window window.cpp)

