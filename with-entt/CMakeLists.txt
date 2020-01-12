cmake_minimum_required(VERSION 3.0)
project(gol-with-entt)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory("rapidcheck")

add_executable(gol main.cpp systems.cpp components.cpp utils.cpp)
target_link_libraries(gol ${CONAN_LIBS})
if(CMAKE_COMPILER_IS_GNUCXX)
  target_compile_options(gol PRIVATE "-Wall")
elseif(MSVC)
  target_compile_options(gol PRIVATE "/W3")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(gol PRIVATE "-Wall")
endif()

if(NOT ${CMAKE_BUILD_TYPE} STREQUAL "Release" AND NOT DEFINED GOL_NO_PROF)
  target_link_libraries(gol "profiler")
endif()
if (DEFINED GOL_ALLOCATOR)
  if (${GOL_ALLOCATOR} STREQUAL "tcmalloc")
    target_link_libraries(gol "tcmalloc")
    if (CMAKE_COMPILER_IS_GNUCXX)
      target_compile_options(gol PRIVATE
        "-fno-builtin-malloc" "-fno-builtin-calloc" "-fno-builtin-realloc" "-fno-builtin-free")
    endif()
  else()
    message(FATAL_ERROR "Allocator ${GOL_ALLOCATOR} is not handled")
  endif()
endif()

if(${GOL_NO_LOG} STREQUAL "on")
  target_compile_definitions(gol PRIVATE "GOL_NO_LOG")
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

  set(TEST_EXE_NAME run_${TEST_NAME}_tests)
  set(CTEST_NAME ${TEST_NAME}_tests)

  add_executable(${TEST_EXE_NAME} ${TEST_NAME}_test.cpp ${TEST_NAME}.cpp ${TEST_DEPS})
  target_link_libraries(${TEST_EXE_NAME} ${CONAN_LIBS})
  target_link_libraries(${TEST_EXE_NAME} rapidcheck)
  target_compile_definitions(${TEST_EXE_NAME} PRIVATE "GOL_NO_LOG")
  target_compile_definitions(${TEST_EXE_NAME} PRIVATE "RC_USE_RTTI")
  target_include_directories(${TEST_EXE_NAME} PRIVATE "rapidcheck/extras/gtest/include")
  add_test(${CTEST_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_EXE_NAME})
endfunction(add_gol_test)

add_gol_test(NAME systems DEPS components.cpp utils.cpp)
add_gol_test(NAME utils DEPS components.cpp)
add_gol_test(NAME components)
