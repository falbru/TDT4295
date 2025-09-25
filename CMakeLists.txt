cmake_minimum_required(VERSION 3.10)
project(quickdraw VERSION 0.0.1)

option(BUILD_DESKTOP "Build desktop version" ON)

add_subdirectory(lib/gui)

if(BUILD_DESKTOP)
    add_subdirectory(platform/desktop)
endif()
