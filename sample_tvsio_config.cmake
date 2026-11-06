get_filename_component(TVSIO_TVM_FILE_DIR ${MISSION_SOURCE_DIR}/apps/inc/tvm_files ABSOLUTE)

# SET TVSIO TVM file list for CPU1. not needed for this example but shows how to do it.
if(TGT_NAME STREQUAL "cpu1")
set(TVM_FILES
ranger_cmd.tvm
ranger_tlm.tvm
temp_controller_cmd.tvm
temp_controller_tlm.tvm
)
endif()

foreach(TVM_FILE ${TVM_FILES})
list(APPEND TVSIO_TVM_FILES ${TVSIO_TVM_FILE_DIR}/${TVM_FILE})
endforeach()

list(APPEND TVSIO_TVM_INCLUDE_DIRS
${MISSION_SOURCE_DIR}/apps/inc
${MISSION_SOURCE_DIR}/apps/temp_mon/fsw/src
)