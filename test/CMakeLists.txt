macro(add_unit_test SUBDIR TARGET)
    add_executable(${TARGET} ${SUBDIR}/${TARGET}.cpp)
    add_test(${TARGET} ${TARGET})
    target_include_directories(${TARGET} PRIVATE gtest)
    target_link_libraries(${TARGET} ray gtest gtest_main pthread)
endmacro(add_unit_test)

add_unit_test(platform PrintTests)
add_unit_test(assets BitmapTests)
add_unit_test(math ScalarTests)
add_unit_test(math Vector2Tests)
add_unit_test(math Vector3Tests)
add_unit_test(math Vector4Tests)
add_unit_test(math MatrixTests)
add_unit_test(math QuaternionTests)


