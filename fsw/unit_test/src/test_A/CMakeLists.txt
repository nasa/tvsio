cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")

include_directories(BEFORE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(TEST_SOURCES
    ${COMMON_TEST_SRC_FILES}
    ${CMAKE_CURRENT_SOURCE_DIR}/tvs_io_generated.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tvmc_test_A.c
)

add_executable(tvmc_test_A ${TEST_SOURCES})