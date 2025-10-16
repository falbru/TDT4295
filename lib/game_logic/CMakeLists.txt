cmake_minimum_required(VERSION 3.5)
project(game_logic)

add_library(game_logic src/drawings.c)

target_include_directories(game_logic PUBLIC include/)