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
add_executable(rectangle rectangle.cpp)

