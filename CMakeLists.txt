project (pbstru)
cmake_minimum_required (VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
# set(CMAKE_EXE_LINKER_FLAGS "-static -pthread")

aux_source_directory(./pbstru SRC_FILES)

include_directories(
	/usr/local/include
	/usr/local/include/google/protobuf/compiler
	protobuf3/src
	protobuf3/src/google/protobuf/compiler
)

####################
message("**** CMAKE_CURRENT_SOURCE_DIR=" ${CMAKE_CURRENT_SOURCE_DIR})
message("**** CMAKE_CURRENT_BINARY_DIR=" ${CMAKE_CURRENT_BINARY_DIR})

IF (WIN32)
add_custom_target(
    protobuf_lib
    COMMAND mingw32-make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/cmake/build
)
ELSE()
add_custom_target(
    protobuf_lib
    # COMMAND ./autogen.sh && ./configure && make
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/protobuf3
)
ENDIF()

IF (WIN32)
	message("${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/cmake/build")
	find_library(PROTOBUF_LIB NAMES protobuf HINTS "${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/cmake/build" )
ELSE()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	find_library(PROTOBUF_LIB NAMES protobuf HINTS "${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/src/.libs" )
ENDIF()
set (EXTRA_LIBS ${EXTRA_LIBS} ${PROTOBUF_LIB})

# add the executable
add_executable(pbstru ${SRC_FILES})
target_link_libraries (pbstru  ${EXTRA_LIBS})
add_dependencies(pbstru protobuf_lib)
