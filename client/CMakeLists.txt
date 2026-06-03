cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(
    client

    main.c
)

include_directories(".")
