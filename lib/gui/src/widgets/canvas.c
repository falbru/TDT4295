#include "widgets/canvas.h"
#include "color.h"
#include "framebuffer.h"
#include "primitives/rectangle.h"
#include "widgets/widget.h"
#include <stdlib.h>
#include <string.h>

static void canvas_render_callback(Widget *widget, Framebuffer *framebuffer);
static void canvas_destroy_callback(Widget *widget);
static void canvas_dirty_callback(Widget *widget, Framebuffer * framebuffer);
static void expand_dirty_rect(CanvasData *data, int x, int y, int width, int height);

Widget *canvas_create(int x, int y, int width, int height)
{
    Widget *canvas = (Widget *)malloc(sizeof(Widget));
    if (!canvas)
        return NULL;

    widget_init(canvas, WIDGET_TYPE_CANVAS, x, y, width, height);

    CanvasData *data = (CanvasData *)malloc(sizeof(CanvasData));
    if (!data)
    {
        free(canvas);
        return NULL;
    }

    int pixel_count = width * height;
    data->pixels = (uint8_t *)malloc(pixel_count * sizeof(uint8_t));
    if (!data->pixels)
    {
        free(data);
        free(canvas);
        return NULL;
    }

    for (int i = 0; i < pixel_count; i++)
    {
        data->pixels[i] = 0;
    }

    data->brush_size = 3;
    data->background_color = COLOR_WHITE;
    data->brush_color = COLOR_BLACK;
    data->border_color = COLOR_BLACK;
    data->border_thickness = 2;
    data->dirty_x = 0;
    data->dirty_y = 0;
    data->dirty_width = 0;
    data->dirty_height = 0;
    data->has_dirty_rect = 0;

    canvas->data = data;

    canvas->render = canvas_render_callback;
    canvas->destroy = canvas_destroy_callback;
    canvas->on_dirty = canvas_dirty_callback;

    return canvas;
}

static void canvas_dirty_callback(Widget *widget, Framebuffer* framebuffer)
{
    if(!widget || !framebuffer)
        return;

    CanvasData *data = (CanvasData *)widget->data;
    if (!data || !data->has_dirty_rect)
        return;

    if (framebuffer->dirty_rect_count < MAX_DIRTY_RECTS)
    {
        DirtyRect *rect = &framebuffer->dirty_rects[framebuffer->dirty_rect_count];
        rect->x = widget->x + data->dirty_x;
        rect->y = widget->y + data->dirty_y;
        rect->width = data->dirty_width;
        rect->height = data->dirty_height;
        framebuffer->dirty_rect_count++;
    }
}

static void canvas_render_callback(Widget *widget, Framebuffer *framebuffer)
{
    if (!widget || !widget->data)
        return;

    CanvasData *data = (CanvasData *)widget->data;

    for (int row = 0; row < widget->height; row++)
    {
        for (int col = 0; col < widget->width; col++)
        {
            int canvas_idx = row * widget->width + col;
            int fb_x = widget->x + col;
            int fb_y = widget->y + row;

            if (fb_x >= 0 && fb_x < FRAMEBUFFER_WIDTH(framebuffer) && fb_y >= 0 && fb_y < FRAMEBUFFER_HEIGHT(framebuffer))
            {
                Color c = COLOR_WHITE;
                if (data->pixels[canvas_idx] > 0) c= COLOR_BLACK;
                FRAMEBUFFER_SET_PIXEL(framebuffer, fb_x, fb_y, c);
            }
        }
    }

    if (data->border_thickness > 0)
    {
        renderRectangle(widget->x, widget->y, widget->width, widget->height, data->border_color, data->border_thickness,
                        framebuffer);
    }

    data->has_dirty_rect = 0;
    data->dirty_x = 0;
    data->dirty_y = 0;
    data->dirty_width = 0;
    data->dirty_height = 0;
}

static void canvas_destroy_callback(Widget *widget)
{
    if (!widget || !widget->data)
        return;

    CanvasData *data = (CanvasData *)widget->data;

    if (data->pixels)
    {
        free(data->pixels);
        data->pixels = NULL;
    }
}

static void expand_dirty_rect(CanvasData *data, int x, int y, int width, int height)
{
    if (!data)
        return;

    if (!data->has_dirty_rect)
    {
        data->dirty_x = x;
        data->dirty_y = y;
        data->dirty_width = width;
        data->dirty_height = height;
        data->has_dirty_rect = 1;
    }
    else
    {
        int right = data->dirty_x + data->dirty_width;
        int bottom = data->dirty_y + data->dirty_height;
        int new_right = x + width;
        int new_bottom = y + height;

        if (x < data->dirty_x)
            data->dirty_x = x;
        if (y < data->dirty_y)
            data->dirty_y = y;
        if (new_right > right)
            right = new_right;
        if (new_bottom > bottom)
            bottom = new_bottom;

        data->dirty_width = right - data->dirty_x;
        data->dirty_height = bottom - data->dirty_y;
    }
}

void canvas_set_brush_size(Widget *canvas, int size)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data)
        return;

    data->brush_size = size > 0 ? size : 1;
}

void canvas_set_brush_color(Widget *canvas, Color color)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data)
        return;

    data->brush_color = color;
}

void canvas_set_background_color(Widget *canvas, Color color)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data)
        return;

    data->background_color = color;
}

void canvas_set_border(Widget *canvas, Color color, int thickness)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data)
        return;

    data->border_color = color;
    data->border_thickness = thickness;
}

void canvas_draw_at(Widget *canvas, int x, int y)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data || !data->pixels)
        return;

    int canvas_x = x - canvas->x;
    int canvas_y = y - canvas->y;

    if (canvas_x < 0 || canvas_x >= canvas->width || canvas_y < 0 || canvas_y >= canvas->height)
    {
        return;
    }

    int half_brush = data->brush_size / 2;

    int min_x = canvas_x - half_brush;
    int min_y = canvas_y - half_brush;
    int max_x = canvas_x + half_brush;
    int max_y = canvas_y + half_brush;

    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x >= canvas->width) max_x = canvas->width - 1;
    if (max_y >= canvas->height) max_y = canvas->height - 1;

    for (int dy = -half_brush; dy <= half_brush; dy++)
    {
        for (int dx = -half_brush; dx <= half_brush; dx++)
        {
            int px = canvas_x + dx;
            int py = canvas_y + dy;

            if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height)
            {
                int idx = py * canvas->width + px;
                data->pixels[idx] = 255;
            }
        }
    }

    expand_dirty_rect(data, min_x, min_y, max_x - min_x + 1, max_y - min_y + 1);

    widget_mark_dirty(canvas);
}

void canvas_clear(Widget *canvas)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data || !data->pixels)
        return;

    int pixel_count = canvas->width * canvas->height;
    for (int i = 0; i < pixel_count; i++)
    {
        data->pixels[i] = 0;
    }

    expand_dirty_rect(data, 0, 0, canvas->width, canvas->height);

    widget_mark_dirty(canvas);
}
