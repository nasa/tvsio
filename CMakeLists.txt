cmake_minimum_required(VERSION 3.26)
project(TVSIO_APP C CXX)
#include(CMakePrintHelpers) # for debugging build
find_package(Python COMPONENTS Interpreter)


set(TVSIO_INCLUDE_DIRS
fsw/platform_inc
fsw/mission_inc
fsw/src
${CMAKE_CURRENT_BINARY_DIR}
)

set(TVSIO_LINK_LIBRARIES m)

# Set empty user-provided cmake vars
set(TVSIO_TVM_INCLUDE_DIRS)
set(TVSIO_TVM_LINK_LIBRARIES)

set(APP_SRC_FILES
    fsw/src/tvsio_app.c
    fsw/src/tvsio_utils.c
    ${CMAKE_CURRENT_BINARY_DIR}/tvsio_generated.c
)

add_custom_target(
    run_tvmc ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tvsio_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvsio_generated.h
    COMMENT "Generates code from *.tvm files during tvsio build"
)

set(SPECIFIED_CONFIG ${BUILD_CONFIG_${TARGETSYSTEM}})
list(GET SPECIFIED_CONFIG 2 TGT_NAME)
list(GET SPECIFIED_CONFIG 0 PLATFORM_NAME)

if (EXISTS "${MISSION_DEFS}/tvsio_config.cmake")
set(TVSIO_TVM_FILES)
include(${MISSION_DEFS}/tvsio_config.cmake)
else()
message(WARNING "No ${MISSION_DEFS}/tvsio_config.cmake detected. Using pre-defined includes and tvm file directory locations.")
FILE(GLOB TVSIO_TVM_FILES ${MISSION_SOURCE_DIR}/apps/inc/tvm_files/*.tvm)
endif()

if(NOT TVSIO_TVM_FILES)
message(FATAL_ERROR "No TVM files found!")
else()
message(STATUS "TVS tvmc will process")
foreach(file IN LISTS TVSIO_TVM_FILES)
  message(STATUS "  ${file}")
endforeach()
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tvsio_generated.c ${CMAKE_CURRENT_BINARY_DIR}/tvsio_generated.h
    COMMAND ${Python_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/tvmc/tvmc.py" ${TVSIO_TVM_FILES} "-o" "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS 
        ${TVSIO_TVM_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/tvmc/tvmc.py
)

# Create the app module
add_cfe_app(tvsio ${APP_SRC_FILES})

add_dependencies(tvsio run_tvmc)

target_link_libraries(tvsio ${TVSIO_TVM_LINK_LIBRARIES} ${TVSIO_LINK_LIBRARIES})

target_include_directories(tvsio PUBLIC
    ${TVSIO_TVM_INCLUDE_DIRS} ${TVSIO_INCLUDE_DIRS}
)
