project (pbstru)
cmake_minimum_required (VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE OFF)

IF (MSVC)
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
ELSE()
  set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wsign-compare -Wmissing-prototypes -Wpacked -Wunreachable-code -Winline")
  set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wpointer-arith -Wcast-align -Wsign-compare -Wpacked -Wunreachable-code -Winline -Wno-unused-parameter")
#   set(CMAKE_EXE_LINKER_FLAGS "-static -pthread")
ENDIF (MSVC)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

aux_source_directory(./src SRC_FILES)

include_directories(
	/usr/local/include
	/usr/local/include/google/protobuf/compiler
	protobuf3/src
	protobuf3/src/google/protobuf/compiler
)

####################
message("**** {CMAKE_CURRENT_SOURCE_DIR}=" ${CMAKE_CURRENT_SOURCE_DIR})
message("**** {CMAKE_CURRENT_BINARY_DIR}=" ${CMAKE_CURRENT_BINARY_DIR})

IF (MSVC)
add_custom_target(
    protobuf_lib
    COMMAND msbuild.exe protoc.vcxproj
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/cmake/build
)
ELSE()
add_custom_target(
    protobuf_lib
    # COMMAND ./autogen.sh && ./configure && make
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/protobuf3
)
add_custom_target(
    auto_increment_version
    COMMAND ${CMAKE_COMMAND} -D VERSION_FILE=${CMAKE_SOURCE_DIR}/src/version.h -P ${CMAKE_SOURCE_DIR}/AutoVersion.cmake
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
ENDIF (MSVC)

IF (MSVC)
    find_library(PROTOBUF_LIB NAMES libprotobufd HINTS "${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/cmake/build/Debug" )
ELSE()
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    find_library(PROTOBUF_LIB NAMES protobuf HINTS "${CMAKE_CURRENT_SOURCE_DIR}/protobuf3/src/.libs" )
ENDIF (MSVC)
set (EXTRA_LIBS ${EXTRA_LIBS} ${PROTOBUF_LIB})

# add the executable
add_executable(pbstru ${SRC_FILES})
target_link_libraries (pbstru ${EXTRA_LIBS})

IF (MSVC)
  add_dependencies(pbstru protobuf_lib)
ELSE()
  add_dependencies(pbstru protobuf_lib)
  add_dependencies(pbstru auto_increment_version)
ENDIF (MSVC)
