cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

include_directories(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TEST_SOURCES
    ${COMMON_TEST_SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/tvmc_test_example.c    
)

add_executable(tvs_io_test_example ${TEST_SOURCES})