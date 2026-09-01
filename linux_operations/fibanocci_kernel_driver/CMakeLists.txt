cmake_minimum_required(VERSION 3.10)
project(fibonacci_module)

# Kernel module configuration
set(KERNEL_DIR /lib/modules/${CMAKE_SYSTEM_VERSION}/build)
set(MODULE_NAME fibonacci)

# Source files
set(FIBONACCI_SOURCES src/fibonacci.c)

# Generate a Makefile for kernel module building
file(
        WRITE ${CMAKE_CURRENT_BINARY_DIR}/Makefile
        "\
obj-m += fibonacci.o\n\
fibonacci-objs := src/fibonacci.o\n\
all:\n\tmake -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules\n\
clean:\n\tmake -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean\n"
)

# Build kernel module using the generated Makefile
add_custom_target(
        kernel_module_build
        COMMAND make -C /lib/modules/${CMAKE_SYSTEM_VERSION}/build M=${CMAKE_CURRENT_BINARY_DIR} modules
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Installation target
add_custom_target(
        install_modules
        COMMAND sudo insmod ${MODULE_NAME}.ko
        DEPENDS kernel_module_build
)

# Upload target
add_custom_target(
        upload
        COMMAND sudo mknod /dev/fibonacci c 510 0
        COMMAND ls /dev/fibonacci
        DEPENDS install_modules
)

# Uninstallation target
add_custom_target(uninstall_modules COMMAND sudo rmmod ${MODULE_NAME})

# Reinstall target
add_custom_target(
        reinstall_modules
        COMMAND make -C ${KERNEL_DIR} M=${CMAKE_CURRENT_BINARY_DIR} clean
        COMMAND make -C ${KERNEL_DIR} M=${CMAKE_CURRENT_BINARY_DIR} modules
        COMMAND sudo rmmod ${MODULE_NAME}
        COMMAND sudo insmod ${MODULE_NAME}.ko
)

# Test executables
add_executable(test-with-gmp src/fibonacciCalc.c src/test-with-gmp.c)
target_link_libraries(test-with-gmp gmp)
target_include_directories(test-with-gmp PRIVATE include)

add_executable(test-without-gmp src/test-without-gmp.c)

# Clean target
add_custom_target(
        clean_all
        COMMAND make -C ${KERNEL_DIR} M=${CMAKE_CURRENT_BINARY_DIR} clean
        COMMAND rm -f test-without-gmp test-with-gmp
        DEPENDS uninstall_modules
)
