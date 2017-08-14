
file(GLOB_RECURSE CppUtilTests_SRC
        "*.cpp"
        "*.hpp"
        "../include/CppUtils/*.hpp"
        )

add_executable(CppUtilTests ${CppUtilTests_SRC})

target_link_libraries(CppUtilTests LINK_PUBLIC gtest gmock)

# The custom target and custom command below allow the unit tests
# to be run.
# If you want them to run automatically by CMake, uncomment #ALL
add_custom_target(
        run_unit_tests #ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/CppUtilTests.touch CppUtilTests)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/CppUtilTests.touch
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/CppUtilTests)