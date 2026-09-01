cmake_minimum_required(VERSION 3.15)
project(StashProject)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror")

add_executable(stash src/functions.c src/main.c)
target_link_libraries(stash PRIVATE crypto)

add_custom_target(
    generateKey
    COMMAND openssl enc -aes-256-cbc -pass file:password.txt -P -pbkdf2 -md sha256 -nosalt > key.bin
    COMMENT "Generating encryption key"
)
