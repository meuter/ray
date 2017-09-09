project(ray CXX)
cmake_minimum_required(VERSION 3.0)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
add_subdirectory(contrib/glfw)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(samples)
