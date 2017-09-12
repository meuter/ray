project(ray)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

###### Boost
find_package(Boost COMPONENTS system filesystem REQUIRED)

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

######
add_subdirectory(contrib/tinyobjloader)


###### Ray
add_library(ray 
    src/ray/assets/Bitmap.cpp
    src/ray/gl/Texture.cpp
    src/ray/platform/FileSystem.cpp
)

target_include_directories(ray
    PUBLIC
        include
    PRIVATE
        ${Boost_FILESYSTEM_INCLUDE_DIRS}
)
message(" - boost fs libraries = ${Boost_FILESYSTEM_LIBRARY}")
message(" - boost sys libraries = ${Boost_SYSTEM_LIBRARY}")
target_link_libraries(ray 
    PUBLIC
        stb
        glfw
        glad 
        tinyobjloader
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
)

###### Samples
add_subdirectory(samples)

###### Tests
enable_testing()
add_subdirectory(contrib/googletest)
add_subdirectory(test)
