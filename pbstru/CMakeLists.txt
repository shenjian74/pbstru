project (pbstru)
cmake_minimum_required (VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE ON)


aux_source_directory(./pbstru SRC_FILES)
aux_source_directory(../bstrlib SRC_FILES)
list(REMOVE_ITEM SRC_FILES "../bstrlib/test.cpp")
list(REMOVE_ITEM SRC_FILES "../bstrlib/testaux.c")
list(REMOVE_ITEM SRC_FILES "../bstrlib/bstest.c")

include_directories(
	../bstrlib
	../protobuf3/src
	../protobuf3/src/google/protobuf/compiler
)

find_library(PROTOBUF_LIB NAMES protobuf
             HINTS "../protobuf3/src/.libs" )
set (EXTRA_LIBS ${EXTRA_LIBS} ${PROTOBUF_LIB})

# add the executable
add_executable(pbstru ${SRC_FILES})
target_link_libraries (pbstru  ${EXTRA_LIBS})
