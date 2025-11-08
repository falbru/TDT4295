cmake_minimum_required(VERSION 3.10)
project(quickdraw VERSION 0.0.1)

option(BUILD_DESKTOP "Build desktop version" ON)
option(BUILD_TESTS "Build tests" ON)

add_subdirectory(lib/gui)

add_subdirectory(lib/game)

if(BUILD_DESKTOP)
    add_subdirectory(platform/desktop)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(vendor/Unity)
    add_subdirectory(test)
endif()
