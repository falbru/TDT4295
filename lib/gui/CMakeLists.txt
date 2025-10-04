cmake_minimum_required(VERSION 3.5)
project(gui)

add_library(gui src/text.c src/image.c)

target_include_directories(gui PUBLIC include/)
