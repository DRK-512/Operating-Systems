cmake_minimum_required(VERSION 3.15)
project(linux_apps)

# Globally used variables
set(TOPDIR
    ${CMAKE_CURRENT_SOURCE_DIR}
    CACHE STRING "Top level directory"
)
set(SCRIPT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/scripts/
    CACHE STRING "Scripts directory"
)

# I do not want RPATHs on target unless we are in Debug mode
# This is only a C++ thing not a rust one, so it would be be libs
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_SKIP_RPATH TRUE)
        set(CMAKE_INSTALL_RPATH "")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--disable-new-dtags")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--disable-new-dtags")
else()
        # The thing is, if we are in debug mode, I want to add the debug flag
        add_compile_options($<$<COMPILE_LANGUAGE:C>:-g>)
endif()

add_custom_target(build_all ALL)

set(PROJECT_DIRS "linux_operations")
#set(PROJECT_DIRS "linux_operations;process_synchronization;scheduling;memory_management;networking;security;misc")

# Now I bring in the cmake files so I can call the functions below
# They also set the variables SAST and FORMAT_CHECKS
include(cmake/build_options.cmake)
include(cmake/format_checker.cmake)

# I check the projects formats are correct before I compile them
if(FORMAT_CHECKS)
        check_format(build_all)
endif()

foreach(SUBDIR IN LISTS PROJECT_DIRS)
        add_subdirectory(${SUBDIR})
endforeach()
