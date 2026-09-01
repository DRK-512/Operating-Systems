cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME modifed_rm)
project(${PROJ_NAME})

# Add the executable
add_executable(${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c ${CMAKE_CURRENT_SOURCE_DIR}/src/functions.c)

target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_compile_options(
        ${PROJ_NAME}
        -Wall
        -Werror
        -Wextra
        -Wpedantic
        -Wshadow
        -Wformat
)
