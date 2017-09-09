project(ray)
cmake_minimum_required(VERSION 3.0)

###### OpenGL
find_package(OpenGL REQUIRED)

###### GLFW
set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
add_subdirectory(contrib/glfw)

###### GLAD
add_library(glad contrib/glfw/deps/glad.c)
target_include_directories(glad PUBLIC contrib/glfw/deps/)

###### Samples
add_subdirectory(samples)
