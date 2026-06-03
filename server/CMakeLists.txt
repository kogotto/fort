cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(
    server

    main.cpp
)

include_directories(".")
