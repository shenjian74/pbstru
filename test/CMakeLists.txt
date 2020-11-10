set(PROJECT_NAME "test_codec")

project (${PROJECT_NAME})
cmake_minimum_required (VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE OFF)

IF (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DXOS_snprintf=snprintf")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DXOS_snprintf=snprintf")
    foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      endif(${flag_var} MATCHES "/MD")
    endforeach(flag_var)
ELSE ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -DXOS_snprintf=snprintf")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -DXOS_snprintf=snprintf")
    # set(CMAKE_EXE_LINKER_FLAGS "-static -pthread")
ENDIF (MSVC)

aux_source_directory("." SRC_FILES1)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../bin/codec/source" SRC_FILES1)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../bin/codec/include
)

add_definitions(-D_TEST_CODEC)

# add the executable
add_executable(${PROJECT_NAME} ${SRC_FILES1})
