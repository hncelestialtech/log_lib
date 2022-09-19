cmake_minimum_required(VERSION 3.12)
project(log_lib)

if (NOT LOGGER)
    message("no LOGGER set, set to default spdlogger")
    set(LOGGER "SPDLOGGER")
endif ()

add_subdirectory(src)