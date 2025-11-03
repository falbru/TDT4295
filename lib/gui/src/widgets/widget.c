#include "widgets/widget.h"
#include "widgets/container.h"
#include <stdlib.h>
#include <string.h>

void widget_init(Widget *widget, WidgetType type, int x, int y, int width, int height)
{
    if (!widget)
        return;

    memset(widget, 0, sizeof(Widget));

    widget->type = type;
    widget->x = x;
    widget->y = y;
    widget->width = width;
    widget->height = height;
    widget->visible = true;
    widget->enabled = true;
    widget->on_click = NULL;
    widget->render = NULL;
    widget->destroy = NULL;
    widget->user_data = NULL;
    widget->data = NULL;
}

void widget_destroy(Widget *widget)
{
    if (!widget)
        return;

    if (widget->destroy)
    {
        widget->destroy(widget);
    }

    if (widget->data)
    {
        free(widget->data);
        widget->data = NULL;
    }
}

void widget_render(Widget *widget, Framebuffer *framebuffer)
{
    if (!widget || !framebuffer)
        return;
    if (!widget->visible)
        return;

    if (widget->render)
    {
        widget->render(widget, framebuffer);
    }
}

void widget_set_position(Widget *widget, int x, int y)
{
    if (!widget)
        return;
    widget->x = x;
    widget->y = y;
}

void widget_set_size(Widget *widget, int width, int height)
{
    if (!widget)
        return;
    widget->width = width;
    widget->height = height;

    if (widget->type == WIDGET_TYPE_CONTAINER) {
        container_update_layout(widget);
    }
}

void widget_set_visible(Widget *widget, bool visible)
{
    if (!widget)
        return;
    widget->visible = visible;
}

void widget_set_enabled(Widget *widget, bool enabled)
{
    if (!widget)
        return;
    widget->enabled = enabled;
}

bool widget_contains_point(Widget *widget, int x, int y)
{
    if (!widget)
        return false;

    return (x >= widget->x && x < widget->x + widget->width && y >= widget->y && y < widget->y + widget->height);
}

void widget_handle_click(Widget *widget, int x, int y)
{
    if (!widget)
        return;
    if (!widget->visible || !widget->enabled)
        return;

    if (widget_contains_point(widget, x, y))
    {
        if (widget->on_click)
        {
            widget->on_click(widget, widget->user_data);
        }
    }
}
