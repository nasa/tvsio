cmake_minimum_required(VERSION 2.6.4)
project(CFE_TVS_IO_APP C)

find_package( PythonInterp 2.7 REQUIRED )

set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

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
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.h mybogusfile.h
    COMMENT "Generate code from *.tvm files during tvs_io build"
)

set(TVM_FILE_DIR_ARG $ENV{TVM_FILE_DIR})

if (NOT DEFINED TVM_FILE_DIR_ARG)

    set(TVS_IO_TVM_FILE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../inc/tvm_files/*")

else()

    get_filename_component(TVS_IO_TVM_FILE_DIR ${TVM_FILE_DIR_ARG} ABSOLUTE)
    set(TVS_IO_TVM_FILE_DIR "${TVS_IO_TVM_FILE_DIR}/*")

endif()

# had to add "mybogusfile.h" which is never generated so the custom target above would always force this command to run
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.h mybogusfile.h
    COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tvmc/tvmc.py" ${TVS_IO_TVM_FILE_DIR} "-o" "${CMAKE_CURRENT_BINARY_DIR}"
)

# Create the app module
add_cfe_app(tvs_io ${APP_SRC_FILES})

add_dependencies(tvs_io run_tvmc)
