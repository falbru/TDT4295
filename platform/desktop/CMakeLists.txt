cmake_minimum_required(VERSION 3.5)
project(desktop)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

set(FONT_BDF ${CMAKE_SOURCE_DIR}/assets/spleen.bdf)
set(FONT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/font.h)
set(BDF2C_SCRIPT ${CMAKE_SOURCE_DIR}/tools/bdf2c.py)

add_custom_command(
    OUTPUT ${FONT_HEADER}
    COMMAND ${CMAKE_COMMAND} -E env python3 ${BDF2C_SCRIPT} ${FONT_BDF} --name font > ${FONT_HEADER}
    DEPENDS ${FONT_BDF} ${BDF2C_SCRIPT}
    COMMENT "Generating font.h from spleen.bdf"
)

add_custom_target(generate_font DEPENDS ${FONT_HEADER})

add_executable(desktop WIN32 src/main.c ${FONT_HEADER})

add_dependencies(desktop generate_font)

target_include_directories(desktop PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(desktop PRIVATE SDL3::SDL3 gui)
