cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(
    client

    main.c
    parse.c
    load.c
    serialize.c
)

include_directories(".")

target_link_libraries(client PRIVATE Threads::Threads m)
