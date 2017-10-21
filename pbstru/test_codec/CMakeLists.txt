set(PROJECT_NAME "test_codec")

project (${PROJECT_NAME})
cmake_minimum_required (VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE ON)

aux_source_directory(. SRC_FILES1)
aux_source_directory(../bin/codec SRC_FILES1)

include_directories(
    ../bin/codec
)

add_definitions(-D_TEST_CODEC)
####################
add_custom_target(
   auto_gen_src
   COMMAND ./gen_codec.sh
   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES1})
add_dependencies(${PROJECT_NAME} auto_gen_src)

