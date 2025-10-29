cmake_minimum_required(VERSION 3.5)
project(gui)

add_library(gui
    src/primitives/text.c
    src/primitives/image.c
    src/primitives/rectangle.c
    src/widgets/widget.c
    src/widgets/button.c
    src/widgets/label.c
)

target_include_directories(gui PUBLIC include/)
