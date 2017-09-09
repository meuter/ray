include_directories(
    ../include/
    ../contrib/glfw/include
)

link_libraries(
    glfw
    ${OPENGL_gl_LIBRARY}    
)

add_executable(window window.cpp)
