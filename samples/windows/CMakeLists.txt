if(WIN32)
    add_library(directx INTERFACE)
    target_include_directories(directx INTERFACE "C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Include")
    if(CMAKE_CL_64)
        link_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x64")
    else()
        link_directories("C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86")
    endif()
    
    add_executable(00_hello_msg_box WIN32 00_hello_msg_box.cpp)
    add_executable(01_windows WIN32 01_windows.cpp)
    add_executable(02_triangle WIN32 02_triangle.cpp)
    target_link_libraries(02_triangle PUBLIC directx)
endif()


