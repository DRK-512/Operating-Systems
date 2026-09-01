cmake_minimum_required(VERSION 3.15)
set(PROJ_NAME Portscanner)
project(${PROJ_NAME})

add_executable(
    ${PROJ_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/functions.cpp
)

target_compile_options(
    ${PROJ_NAME}
    PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Werror -Wextra -Wpedantic -Wshadow -Wformat -O2>
)

target_include_directories(${PROJ_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
find_package(SFML 3 COMPONENTS Network System REQUIRED)
target_link_libraries(${PROJ_NAME} PRIVATE SFML::Network SFML::System)

install(TARGETS ${PROJ_NAME} RUNTIME DESTINATION bin)
