set(PROJECT_NAME "test_codec")

project (${PROJECT_NAME})
cmake_minimum_required (VERSION 2.6)
# set(CMAKE_VERBOSE_MAKEFILE ON)

aux_source_directory("." SRC_FILES1)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../bin/codec/source" SRC_FILES1)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../bin/codec/include
)

add_definitions(-D_TEST_CODEC)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES1})
