project(ray)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

###### OpenGL
find_package(OpenGL REQUIRED)

###### GLFW
set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
add_subdirectory(contrib/glfw)

###### GLAD
add_library(glad contrib/glfw/deps/glad.c)
target_include_directories(glad PUBLIC contrib/glfw/deps/)

###### Ray
add_library(ray INTERFACE)
target_include_directories(ray INTERFACE include/)
target_link_libraries(ray INTERFACE glfw glad)

###### Samples
add_subdirectory(samples)

###### Tests
enable_testing()
add_subdirectory(contrib/googletest)
add_subdirectory(test)
