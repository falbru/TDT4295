#include "framebuffer.h"
#include "widgets/container.h"
#include "widgets/widget.h"
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
    widget->parent = NULL;
    widget->on_click = NULL;
    widget->render = NULL;
    widget->destroy = NULL;
    widget->user_data = NULL;
    widget->data = NULL;
    widget->dirty = true;
    widget->on_dirty = NULL;
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

    if (!widget->visible || !widget->dirty) {
        widget->dirty = false;
        return;
    }

    if (widget->render)
    {
        widget->render(widget, framebuffer);
    }

    widget->dirty = false;
    widget->prev_x = widget->x;
    widget->prev_y = widget->y;
    widget->prev_width = widget->width;
    widget->prev_height = widget->height;
}

void widget_handle_dirty(Widget *widget, Framebuffer *framebuffer)
{
    if (!widget || !widget->dirty)
        return;

    if (widget->on_dirty)
    {
        widget->on_dirty(widget, framebuffer);
    }
    else
    {
        DirtyRect prev_geometry = (DirtyRect){widget->prev_x, widget->prev_y, widget->prev_width, widget->prev_height};
        framebuffer->dirty_rects[framebuffer->dirty_rect_count] = prev_geometry;
        framebuffer->dirty_rect_count++;
    }
}

void widget_set_position(Widget *widget, int x, int y)
{
    if (!widget)
        return;

    if (widget->x == x && widget->y == y)
        return;

    widget->x = x;
    widget->y = y;
    widget_mark_dirty(widget);
}

void widget_set_size(Widget *widget, int width, int height)
{
    if (!widget)
        return;

    if (widget->width == width && widget->height == height)
        return;

    widget->width = width;
    widget->height = height;
    widget_mark_dirty(widget);
}

void widget_set_visible(Widget *widget, bool visible)
{
    if (!widget)
        return;

    if (widget->visible == visible)
        return;

    if (widget->type == WIDGET_TYPE_CONTAINER)
    {
        ContainerData *data = (ContainerData *)widget->data;
        if (data)
        {
            for (int i = 0; i < data->child_count; i++)
            {
                if (data->children[i])
                {
                    widget_set_visible(data->children[i], visible);
                }
            }
        }
    }

    widget->visible = visible;
    widget_mark_dirty(widget);
}

void widget_set_enabled(Widget *widget, bool enabled)
{
    if (!widget)
        return;
    widget->enabled = enabled;
}

void widget_mark_dirty(Widget *widget)
{
    if (!widget)
        return;

    if (widget->parent)
        widget_mark_dirty(widget->parent);

    widget->dirty = true;
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
    if (widget->type == WIDGET_TYPE_CONTAINER)
    {
        ContainerData *data = (ContainerData *)widget->data;
        if (data)
        {
            for (int i = 0; i < data->child_count; i++)
            {
                if (data->children[i])
                {
                    widget_handle_click(data->children[i], x, y);
                }
            }
        }
    }
}
