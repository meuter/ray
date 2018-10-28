project(ray)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

###### Boost
add_subdirectory(contrib/boost-cmake)
add_library(boost INTERFACE)
target_include_directories(boost INTERFACE Boost::filesystem Boost::system)
target_link_libraries(boost INTERFACE Boost::filesystem Boost::system)

###### OpenGL
find_package(OpenGL REQUIRED)

###### DirectX
# TODO(cme): find a more portable way to have this
# if(WIN32)
#     add_library(directx INTERFACE)
#     target_include_directories(directx INTERFACE "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include")
#     if(CMAKE_CL_64)
#         link_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64")
#     else()
#         link_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")
#     endif()
# endif()

macro(turn_on_all_warnings_as_error TARGET)
    if(WIN32)
        target_compile_options(${TARGET} PRIVATE -W2 -WX -DNOMINMAX)
    else()
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Werror)
    endif()    
endmacro()


###### GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
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
turn_on_all_warnings_as_error(ray)

###### Samples
add_subdirectory(samples)

###### Tests
enable_testing()
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(contrib/googletest)
add_subdirectory(test)
