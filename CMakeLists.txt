cmake_minimum_required(VERSION 3.13)
project(CFE_TVS_IO_APP C)
cmake_policy(SET CMP0077 NEW)
#include(CMakePrintHelpers) # for debugging build
find_package(Python COMPONENTS Interpreter)

string(
    CONCAT
    TVS_IO_PER_TARGET_CONFIG_DOC
    "Look for TVM files in target-specific directories; see "
    "https://esgl-gitlab.jsc.nasa.gov/tricksbn/tvsio_app/-/wikis/Per-Target-Configuration"
)
option(TVS_IO_PER_TARGET_CONFIG ${TVS_IO_PER_TARGET_CONFIG_DOC})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../inc) # global 'apps/inc/' directory for CFS mission
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fsw/mission_inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fsw/platform_inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/fsw/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(APP_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/fsw/src/tvs_io_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fsw/src/tvs_io_utils.c
    ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c
)

add_custom_target(
    run_tvmc ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.h
    COMMENT "Generates code from *.tvm files during tvs_io build"
)

set(TVM_FILE_DIR_ARG $ENV{TVM_FILE_DIR})

if (NOT DEFINED TVM_FILE_DIR_ARG)

    set(TVS_IO_TVM_FILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../inc/tvm_files/*")

else()

    get_filename_component(TVS_IO_TVM_FILE_DIR ${TVM_FILE_DIR_ARG} ABSOLUTE)
    if(TVS_IO_PER_TARGET_CONFIG)
        list(LENGTH INSTALL_TARGET_LIST INSTALL_TARGET_COUNT)
        if(NOT ${INSTALL_TARGET_COUNT} EQUAL 1)
            message(FATAL_ERROR "TVS_IO per-target configuration only works with one target in INSTALL_TARGET_LIST")
        endif()
        set(TVS_IO_TVM_FILE_DIR "${TVS_IO_TVM_FILE_DIR}/${INSTALL_TARGET_LIST}/*")
    else()
        set(TVS_IO_TVM_FILE_DIR "${TVS_IO_TVM_FILE_DIR}/*")
    endif()

endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.h
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tvmc/tvmc.py" ${TVS_IO_TVM_FILE_DIR} "-o" "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS 
        ${TVS_IO_TVM_FILE_DIR}
)

# Create the app module
add_cfe_app(tvs_io ${APP_SRC_FILES})

add_dependencies(tvs_io run_tvmc)
