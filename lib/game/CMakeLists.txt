cmake_minimum_required(VERSION 3.5)
project(game)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/font_small.h
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/bdf2c.py --name font_small ${CMAKE_SOURCE_DIR}/assets/spleen-sm.bdf > ${CMAKE_CURRENT_SOURCE_DIR}/include/font_small.h
    DEPENDS ${CMAKE_SOURCE_DIR}/assets/spleen-sm.bdf ${CMAKE_SOURCE_DIR}/tools/bdf2c.py
    COMMENT "Generating font_small.h"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/font_medium.h
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/bdf2c.py --name font_medium ${CMAKE_SOURCE_DIR}/assets/spleen-md.bdf > ${CMAKE_CURRENT_SOURCE_DIR}/include/font_medium.h
    DEPENDS ${CMAKE_SOURCE_DIR}/assets/spleen-md.bdf ${CMAKE_SOURCE_DIR}/tools/bdf2c.py
    COMMENT "Generating font_medium.h"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/include/font_large.h
    COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/bdf2c.py --name font_large ${CMAKE_SOURCE_DIR}/assets/spleen-lg.bdf > ${CMAKE_CURRENT_SOURCE_DIR}/include/font_large.h
    DEPENDS ${CMAKE_SOURCE_DIR}/assets/spleen-lg.bdf ${CMAKE_SOURCE_DIR}/tools/bdf2c.py
    COMMENT "Generating font_large.h"
)

add_custom_target(generate_fonts ALL
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/include/font_small.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/font_medium.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/font_large.h
)

add_library(game
    src/game.c
    src/menu.c
    src/game_page.c
)

add_dependencies(game generate_fonts)

target_include_directories(game PUBLIC include/)

target_link_libraries(game PRIVATE gui)
