cmake_minimum_required(VERSION 3.15)
project(Keylogger)

# Find pkg-config
find_package(PkgConfig REQUIRED)

# Find libevdev
pkg_check_modules(LIBEVDEV REQUIRED libevdev)

# Include directories for libevdev
include_directories(${LIBEVDEV_INCLUDE_DIRS})
add_executable(
    Keylogger
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/KeyloggerFunc.c
)

target_include_directories(
    Keylogger
    PRIVATE
    ${LIBEVDEV_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Keylogger PRIVATE ${LIBEVDEV_LIBRARIES})

