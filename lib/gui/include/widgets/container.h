#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

#include "widgets/widget.h"

typedef enum
{
    LAYOUT_TYPE_NONE,
    LAYOUT_TYPE_HBOX,
    LAYOUT_TYPE_VBOX,
    LAYOUT_TYPE_GRID,
} LayoutType;

typedef enum
{
    ALIGN_START,
    ALIGN_CENTER,
    ALIGN_END,
    ALIGN_STRETCH,
} Alignment;

typedef enum
{
    ANIMATION_NONE,
    ANIMATION_FLOATING,
} AnimationType;

typedef struct
{
    Widget **children;
    int child_count;
    int child_capacity;
    LayoutType layout_type;
    int spacing;
    int padding;
    Alignment alignment;
    Alignment justify;
    int grid_columns;
    AnimationType animation;
    float animation_phase;
    int animation_speed;
} ContainerData;

Widget *container_create(int x, int y, int width, int height, LayoutType layout_type);

void container_add_child(Widget *container, Widget *child);
void container_remove_child(Widget *container, Widget *child);
void container_clear_children(Widget *container);

void container_set_spacing(Widget *container, int spacing);
void container_set_padding(Widget *container, int padding);
void container_set_alignment(Widget *container, Alignment alignment);
void container_set_justify(Widget *container, Alignment justify);
void container_set_grid_columns(Widget *container, int columns);

void container_set_animation(Widget *container, AnimationType animation);
void container_set_animation_speed(Widget *container, int speed);
void container_update_animation(Widget *container, float delta_time);

void container_update_layout(Widget *container);

int container_get_child_count(Widget *container);
Widget *container_get_child(Widget *container, int index);

#endif
