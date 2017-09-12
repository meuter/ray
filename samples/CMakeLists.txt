macro(add_sample TARGET)
    add_executable(${TARGET} ${TARGET}.cpp)
    target_link_libraries(${TARGET} ray)
endmacro(add_sample)

add_sample(window)
add_sample(rectangle)
add_sample(texture)
add_sample(cube)

