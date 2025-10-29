#include "widgets/canvas.h"
#include "primitives/rectangle.h"
#include <stdlib.h>
#include <string.h>

static void canvas_render_callback(Widget *widget, Framebuffer *framebuffer);
static void canvas_destroy_callback(Widget *widget);

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

    memset(data->pixels, COLOR_WHITE, pixel_count);

    data->brush_size = 3;
    data->background_color = COLOR_WHITE;
    data->brush_color = COLOR_BLACK;
    data->border_color = COLOR_BLACK;
    data->border_thickness = 2;

    canvas->data = data;

    canvas->render = canvas_render_callback;
    canvas->destroy = canvas_destroy_callback;

    return canvas;
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

            if (fb_x >= 0 && fb_x < framebuffer->width && fb_y >= 0 && fb_y < framebuffer->height)
            {
                int fb_idx = fb_y * framebuffer->width + fb_x;
                framebuffer->pixels[fb_idx] = data->pixels[canvas_idx];
            }
        }
    }

    if (data->border_thickness > 0)
    {
        renderRectangle(widget->x, widget->y, widget->width, widget->height, data->border_color, data->border_thickness,
                        framebuffer);
    }
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

    for (int dy = -half_brush; dy <= half_brush; dy++)
    {
        for (int dx = -half_brush; dx <= half_brush; dx++)
        {
            int px = canvas_x + dx;
            int py = canvas_y + dy;

            if (px >= 0 && px < canvas->width && py >= 0 && py < canvas->height)
            {
                int idx = py * canvas->width + px;
                data->pixels[idx] = data->brush_color;
            }
        }
    }
}

void canvas_clear(Widget *canvas)
{
    if (!canvas || canvas->type != WIDGET_TYPE_CANVAS)
        return;

    CanvasData *data = (CanvasData *)canvas->data;
    if (!data || !data->pixels)
        return;

    int pixel_count = canvas->width * canvas->height;
    memset(data->pixels, data->background_color, pixel_count);
}
