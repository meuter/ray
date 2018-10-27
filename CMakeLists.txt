project(ray)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

###### setup compiler flags
if (WIN32)
    add_compile_options(-nologo -W3 -DNOMINMAX -D_CRT_SECURE_NO_DEPRECATE -wd4101 -wd4996 -wd4146 -wd4244 -wd4267)
else()
    add_compile_options(-Wall -Wextra -pedantic -Wno-unused -Wno-unused-parameter)
    add_compile_options(-Wno-newline-eof -Wno-gnu-zero-variadic-macro-arguments)
endif()


###### Boost
add_subdirectory(contrib/boost-cmake)
add_library(boost INTERFACE)
target_include_directories(boost INTERFACE Boost::filesystem Boost::system)
target_link_libraries(boost INTERFACE Boost::filesystem Boost::system)

###### OpenGL
find_package(OpenGL REQUIRED)

###### GLFW
set(GLFW_BUILD_EXAMPLES false)
set(GLFW_BUILD_TESTS false)
add_subdirectory(contrib/glfw)

###### GLAD
add_library(glad contrib/glfw/deps/glad.c)
target_include_directories(glad PUBLIC contrib/glfw/deps/)

###### STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE contrib/stb)

###### TinyOBJLoader
add_subdirectory(contrib/tinyobjloader)

###### Ray
add_library(ray 
    src/ray/assets/Bitmap.cpp
    src/ray/assets/Font.cpp
    src/ray/gl/Texture.cpp
    src/ray/components/TextureAtlas.cpp
    src/ray/platform/FileSystem.cpp
)
target_include_directories(ray PUBLIC include)
target_link_libraries(ray PUBLIC stb glfw glad tinyobjloader boost)

###### Samples
add_subdirectory(samples)

###### Tests
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(contrib/googletest)
add_subdirectory(test)
