cmake_minimum_required(VERSION 3.10)
project(virtMem)

add_executable(virMem ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c ${CMAKE_CURRENT_SOURCE_DIR}/src/functions.c)
target_include_directories(virMem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_compile_options(virMem Wall)
