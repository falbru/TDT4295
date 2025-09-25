cmake_minimum_required(VERSION 3.5)
project(desktop)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(desktop WIN32 src/main.c)

target_link_libraries(desktop PRIVATE SDL3::SDL3 gui)
