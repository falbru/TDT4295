cmake_minimum_required(VERSION 3.5)
project(gui)

add_library(gui
    src/primitives/text.c
    src/primitives/image.c
    src/primitives/rectangle.c
    src/widgets/widget.c
    src/widgets/button.c
    src/widgets/label.c
    src/widgets/canvas.c
    src/widgets/container.c
    src/widgets/hbox.c
    src/widgets/vbox.c
    src/widgets/grid.c
)

target_include_directories(gui PUBLIC include/)
