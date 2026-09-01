cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME DiningPhilosophers)
project(${PROJ_NAME})

add_executable(
    ${PROJ_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/philFunc.c
)

target_include_directories(
    ${PROJ_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

add_compile_options(
    -pthread
    -Wall
    -Werror
    -Wextra
    -Wpedantic
    -Wshadow
    -Wformat
)

