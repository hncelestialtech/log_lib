cmake_minimum_required(VERSION 3.12)
project(log_lib C CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../output/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../output/lib)

add_definitions("-Wall")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "No CMAKE_BUILD_TYPE selected, config default value to ${CMAKE_BUILD_TYPE}")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions("-g -O0")
else()
     add_definitions("-O3")
     if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
          #required by butil/crc32.cc to boost performance for 10x
          if ((CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)") AND NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.4))
               add_definitions("-msse4 -msse4.2")
          elseif ((CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64"))
               # segmentation fault in libcontext
                add_definitions("-fno-gcse")
          endif ()
          if (NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0))
               add_definitions("-Wno-aligned-new")
          endif ()
     endif ()
     if (FASTMATH)
          add_definitions(-ffast-math)
     endif()
endif()


aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/common common_src)
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/log_lib log_lib_src)

list(APPEND src ${common_src} ${log_lib_src})

include_directories(${CMAKE_CURRENT_LIST_DIR}/)

if (NOT LOGGER)
     message(FATAL_ERROR "LOGGER not set")
elseif (${LOGGER} STREQUAL SPDLOGGER)
     message(STATUS "spdlog is chosen")
     add_definitions(-DSPDLOGGER)
     add_compile_definitions(SPDLOG_COMPILED_LIB)
     aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/spdlog spdlog_src)
     list(APPEND src ${spdlog_src})
endif()

if (${LOGGER} STREQUAL "SPDLOGGER")
     add_library(spdlog_lib STATIC ${src})
     add_library(spdlog_lib_s SHARED ${src})
     target_link_libraries(spdlog_lib PRIVATE pthread)
     target_link_libraries(spdlog_lib_s PRIVATE pthread)
endif ()

set_target_properties(spdlog_lib_s PROPERTIES VERSION 0.0.0 SOVERSION 0)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/common/ DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../output/include/log_lib/common
     FILES_MATCHING
     PATTERN "*.h"
     PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/log_lib/ DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../output/include/log_lib/log_lib
     FILES_MATCHING
     PATTERN "*.h"
     PATTERN "*.hpp")
file(COPY ${CMAKE_CURRENT_LIST_DIR}/spdlog/ DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../output/include/log_lib/spdlog
     FILES_MATCHING
     PATTERN "*.h"
     PATTERN "*.hpp")
