cmake_minimum_required(VERSION 2.6.4)
project(TVS_IO_TEST C)

set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

# TODO: make it so user can override these at cmd line
set(CFE_PATH ../../../../cfe/fsw/cfe-core)
set(OSAL_PATH ../../../../osal)
set(PSP_PATH ../../../../psp)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR}/inc    
    ${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/inc
    ${CFE_PATH}/../mission_inc
    ${CFE_PATH}/../platform_inc/cpu1
    ${CFE_PATH}/src/es
    ${CFE_PATH}/src/time
    ${CFE_PATH}/src/inc
    ${OSAL_PATH}/src/os/inc
    ${OSAL_PATH}/src/bsp/pc-linux/config
    ${PSP_PATH}/fsw/inc
    ${PSP_PATH}/fsw/pc-linux/inc
)

file(GLOB COMMON_TEST_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ut_assert/src/*.c
)

set(COMMON_TEST_SRC_FILES
    ${COMMON_TEST_SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/tvs_io_utils.c
)

# TODO: instead of rebuilding every object file for each target,
#       build a library here and then link it in the subdirectories below.

add_subdirectory(src/test_example)
add_subdirectory(src/test_A)
add_subdirectory(src/test_amps)