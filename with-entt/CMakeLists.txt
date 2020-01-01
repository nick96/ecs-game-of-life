cmake_minimum_required(VERSION 3.0)
project(gol-with-entt)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(gol-with-entt main.cpp gol.cpp)
target_link_libraries(gol-with-entt ${CONAN_LIBS})

enable_testing()
add_executable(gol-with-entt-test test.cpp gol.cpp)
target_link_libraries(gol-with-entt-test ${CONAN_LIBS})
add_test(tests gol-with-entt-test)
