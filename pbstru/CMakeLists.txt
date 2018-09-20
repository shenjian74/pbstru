project (pbstru)
cmake_minimum_required (VERSION 2.6)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_FLAGS "-O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS "-O0 -Wall -g -ggdb")
set(CMAKE_EXE_LINKER_FLAGS "-static -pthread")

aux_source_directory(./pbstru SRC_FILES)

aux_source_directory("../bstrlib" SRC_FILES)
list(REMOVE_ITEM SRC_FILES "../bstrlib/test.cpp")
list(REMOVE_ITEM SRC_FILES "../bstrlib/testaux.c")
list(REMOVE_ITEM SRC_FILES "../bstrlib/bstest.c")

include_directories(
	../bstrlib
	/usr/local/include
	/usr/local/include/google/protobuf/compiler
	../protobuf3/src
	../protobuf3/src/google/protobuf/compiler
)

add_subdirectory("./test_codec")

IF (WIN32)
add_custom_target(
    protobuf_lib
    COMMAND mingw32-make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf3/cmake/build
)
ELSE()
add_custom_target(
    protobuf_lib
    # COMMAND ./autogen.sh && ./configure && make
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../protobuf3
)
ENDIF()

IF (WIN32)
	message("${CMAKE_CURRENT_SOURCE_DIR}/../protobuf3/cmake/build")
	find_library(PROTOBUF_LIB NAMES protobuf HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf3/cmake/build" )
ELSE()
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	find_library(PROTOBUF_LIB NAMES protobuf HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../protobuf3/src/.libs" )
ENDIF()
set (EXTRA_LIBS ${EXTRA_LIBS} ${PROTOBUF_LIB})

# add the executable
add_executable(pbstru ${SRC_FILES})
target_link_libraries (pbstru  ${EXTRA_LIBS})
add_dependencies(pbstru protobuf_lib)
