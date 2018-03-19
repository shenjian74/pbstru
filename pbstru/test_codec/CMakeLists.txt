set(PROJECT_NAME "test_codec")

project (${PROJECT_NAME})
cmake_minimum_required (VERSION 2.6)
# set(CMAKE_VERBOSE_MAKEFILE ON)

aux_source_directory("." SRC_FILES1)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../bin/codec" SRC_FILES1)

include_directories(
    ../bin/codec
)

add_definitions(-D_TEST_CODEC)

####################
IF(WIN32)
add_custom_target(
    auto_gen_src
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../pbstru.exe . && ${CMAKE_COMMAND} -E remove_directory codec && ${CMAKE_COMMAND} -E make_directory codec && pbstru.exe cdb.proto cdb_ccc.proto codec
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)
ELSE()
add_custom_target(
    auto_gen_src
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/../pbstru . && ./gen_codec.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin
)
ENDIF()

# add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES1})
add_dependencies(${PROJECT_NAME} auto_gen_src)
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
