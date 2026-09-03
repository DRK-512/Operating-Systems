cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME modified_cp)
project(${PROJ_NAME})

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

install(TARGETS ${PROJ_NAME} RUNTIME DESTINATION bin)

# Unit Tests
find_path(UNITY_INCLUDE_DIR
    NAMES unity.h
    PATHS
        /usr/include/unity
        /usr/local/include/unity
)

find_library(UNITY_LIBRARY
    NAMES libunity.a
    PATHS
        /usr/lib
        /usr/local/lib
)

if(NOT UNITY_INCLUDE_DIR OR NOT UNITY_LIBRARY)
    message(FATAL_ERROR "Unity not found")
endif()

add_executable(tests_functions
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit_tests.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/functions.c
)

target_include_directories(tests_functions PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${UNITY_INCLUDE_DIR}
)

target_link_libraries(tests_functions PRIVATE
    ${UNITY_LIBRARY}
)

add_test(
    NAME functions_tests
    COMMAND tests_functions
)
