#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include "framebuffer.h"
#include <stdbool.h>

struct Widget;

typedef enum
{
    WIDGET_TYPE_BUTTON,
    WIDGET_TYPE_LABEL,
    WIDGET_TYPE_CANVAS,
    WIDGET_TYPE_CONTAINER,
    WIDGET_TYPE_PARAGRAPH,
    WIDGET_TYPE_IMAGE,
} WidgetType;

typedef void (*WidgetClickCallback)(struct Widget *widget, void *user_data);
typedef void (*WidgetRenderCallback)(struct Widget *widget, Framebuffer *framebuffer);
typedef void (*WidgetDestroyCallback)(struct Widget *widget);

typedef struct Widget
{
    WidgetType type;

    int x;
    int y;
    int width;
    int height;

    bool visible;
    bool enabled;

    struct Widget *parent;

    WidgetClickCallback on_click;
    WidgetRenderCallback render;
    WidgetDestroyCallback destroy;

    void *user_data;

    void *data;
} Widget;

void widget_init(Widget *widget, WidgetType type, int x, int y, int width, int height);
void widget_destroy(Widget *widget);

void widget_render(Widget *widget, Framebuffer *framebuffer);
void widget_set_position(Widget *widget, int x, int y);
void widget_set_size(Widget *widget, int width, int height);
void widget_set_visible(Widget *widget, bool visible);
void widget_set_enabled(Widget *widget, bool enabled);

bool widget_contains_point(Widget *widget, int x, int y);
void widget_handle_click(Widget *widget, int x, int y);

#endif
