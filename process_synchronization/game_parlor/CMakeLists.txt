cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME GameParlor)
project(${PROJ_NAME})

add_executable(
    ${PROJ_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gameGetters.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gameProcessing.c
)

target_include_directories(
    ${PROJ_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

add_compile_options(${PROJ_NAME} -lpthread -Wall -Werror -Wextra -Wpedantic -Wshadow -Wformat)

