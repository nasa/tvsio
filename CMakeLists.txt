cmake_minimum_required(VERSION 3.26)
project(CFE_TVS_IO_APP C CXX)
#include(CMakePrintHelpers) # for debugging build
find_package(Python COMPONENTS Interpreter)


set(TVS_IO_INCLUDE_DIRS
fsw/platform_inc
fsw/mission_inc
fsw/src
${CMAKE_CURRENT_BINARY_DIR}
)

set(TVS_IO_TVM_INCLUDE_DIRS)

set(APP_SRC_FILES
    fsw/src/tvs_io_app.c
    fsw/src/tvs_io_utils.c
    ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c
)

add_custom_target(
    run_tvmc ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.h
    COMMENT "Generates code from *.tvm files during tvs_io build"
)

set(SPECIFIED_CONFIG ${BUILD_CONFIG_${TARGETSYSTEM}})
list(GET SPECIFIED_CONFIG 2 TGT_NAME)
list(GET SPECIFIED_CONFIG 0 PLATFORM_NAME)

if (EXISTS "${MISSION_DEFS}/tvsio_config.cmake")
set(TVS_IO_TVM_FILES)
include(${MISSION_DEFS}/tvsio_config.cmake)
else()
message(WARNING "No ${MISSION_DEFS}/tvsio_config.cmake detected. Using pre-defined includes and tvm file directory locations.")
FILE(GLOB TVS_IO_TVM_FILES ${MISSION_SOURCE_DIR}/apps/inc/tvm_files/*.tvm)
endif()

if(NOT TVS_IO_TVM_FILES)
message(FATAL_ERROR "No TVM files found!")
else()
message(STATUS "TVS tvmc will process")
foreach(file IN LISTS TVS_IO_TVM_FILES)
  message(STATUS "  ${file}")
endforeach()
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvs_io_generated.h
    COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tvmc/tvmc.py" ${TVS_IO_TVM_FILES} "-o" "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS 
        ${TVS_IO_TVM_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/tvmc/tvmc.py
)

# Create the app module
add_cfe_app(tvs_io ${APP_SRC_FILES})

add_dependencies(tvs_io run_tvmc)


target_include_directories(tvs_io PUBLIC
    ${TVS_IO_INCLUDE_DIRS} ${TVS_IO_TVM_INCLUDE_DIRS}
)
