cmake_minimum_required(VERSION 3.16)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport Core)

add_executable(
    server

    main.cpp
    qcustomplot.cpp
    MainWindow.cpp
    NetThread.cpp
    SyncFlag.cpp
)

include_directories(".")

target_link_libraries(server PRIVATE Qt6::Widgets Qt6::PrintSupport Qt6::Core)
