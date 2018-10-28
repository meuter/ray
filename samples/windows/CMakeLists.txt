macro(add_win32_sample TARGET)
    if(WIN32)
        add_executable(${TARGET} WIN32 ${TARGET}.cpp)
        target_link_libraries(${TARGET} ray directx)
        turn_on_all_warnings_as_error(${TARGET})
    endif()
endmacro(add_win32_sample)

add_win32_sample(00_hello_msg_box)
add_win32_sample(01_windows)
add_win32_sample(02_triangle)


