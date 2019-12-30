cmake_minimum_required(VERSION 3.0)
project(gol-with-entt)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(gol-with-entt main.cpp)
target_link_libraries(gol-with-entt ${CONAN_LIBS})
