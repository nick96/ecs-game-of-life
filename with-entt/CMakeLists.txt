cmake_minimum_required(VERSION 3.0)
project(gol-with-entt)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(gol main.cpp systems.cpp components.cpp utils.cpp)
target_link_libraries(gol ${CONAN_LIBS})

enable_testing()

add_executable(run_tests tests.cpp systems.cpp utils.cpp components.cpp)
target_link_libraries(run_tests ${CONAN_LIBS})
add_test(tests run_tests)
