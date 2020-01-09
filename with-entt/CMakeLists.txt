cmake_minimum_required(VERSION 3.0)
project(gol-with-entt)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(gol main.cpp systems.cpp components.cpp utils.cpp)
target_link_libraries(gol ${CONAN_LIBS})
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(gol PRIVATE "-Wall")
elseif(MSVC)
  target_compile_options(gol PRIVATE "/W3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(gol PRIVATE "-Wall")
endif()


enable_testing()

function(add_gol_test)
  cmake_parse_arguments(
    TEST
    ""
    "NAME"
    "DEPS"
    ${ARGN}
    )

add_executable(run_${TEST_NAME}_tests ${TEST_NAME}_test.cpp ${TEST_NAME}.cpp ${TEST_DEPS})
target_link_libraries(run_${TEST_NAME}_tests ${CONAN_LIBS})
add_test(${TEST_NAME}_tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_${TEST_NAME}_tests)
endfunction(add_gol_test)

add_gol_test(NAME systems DEPS components.cpp utils.cpp)
add_gol_test(NAME utils DEPS components.cpp)
add_gol_test(NAME components)
