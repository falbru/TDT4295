#include "widgets/button.h"
#include "primitives/rectangle.h"
#include "primitives/text.h"
#include <stdlib.h>
#include <string.h>

static void button_render_callback(Widget *widget, Framebuffer *framebuffer);
static void button_destroy_callback(Widget *widget);

Widget *button_create(int x, int y, int width, int height, const char *text)
{
    Widget *button = (Widget *)malloc(sizeof(Widget));
    if (!button)
        return NULL;

    widget_init(button, WIDGET_TYPE_BUTTON, x, y, width, height);

    ButtonData *data = (ButtonData *)malloc(sizeof(ButtonData));
    if (!data)
    {
        free(button);
        return NULL;
    }

    data->text = text ? strdup(text) : NULL;
    data->padding = 8;
    data->background_color = COLOR_GRAY_75;
    data->text_color = COLOR_BLACK;
    data->border_color = COLOR_BLACK;
    data->border_thickness = 1;
    data->font = NULL;

    button->data = data;

    button->render = button_render_callback;
    button->destroy = button_destroy_callback;

    return button;
}

static void button_render_callback(Widget *widget, Framebuffer *framebuffer)
{
    if (!widget || !widget->data)
        return;

    ButtonData *data = (ButtonData *)widget->data;

    renderFilledRectangle(widget->x, widget->y, widget->width, widget->height, data->background_color, framebuffer);

    if (data->border_thickness > 0)
    {
        renderRectangle(widget->x, widget->y, widget->width, widget->height, data->border_color, data->border_thickness,
                        framebuffer);
    }

    if (data->text && data->font)
    {
        int text_x = widget->x + data->padding;
        int text_y = widget->y + data->padding;

        renderText(data->text, data->text_color, text_x, text_y, data->font, framebuffer);
    }
}

static void button_destroy_callback(Widget *widget)
{
    if (!widget || !widget->data)
        return;

    ButtonData *data = (ButtonData *)widget->data;

    if (data->text)
    {
        free(data->text);
        data->text = NULL;
    }
}

void button_set_text(Widget *button, const char *text)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return;

    if (data->text)
    {
        free(data->text);
    }

    data->text = text ? strdup(text) : NULL;
}

void button_set_padding(Widget *button, int padding)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return;

    data->padding = padding;
}

void button_set_background_color(Widget *button, Color color)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return;

    data->background_color = color;
}

void button_set_text_color(Widget *button, Color color)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return;

    data->text_color = color;
}

void button_set_border(Widget *button, Color color, int thickness)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return;

    data->border_color = color;
    data->border_thickness = thickness;
}

void button_set_font(Widget *button, const bdf_font_t *font)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return;

    data->font = font;
}

void button_set_on_click(Widget *button, WidgetClickCallback callback, void *user_data)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    button->on_click = callback;
    button->user_data = user_data;
}

const char *button_get_text(Widget *button)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return NULL;

    ButtonData *data = (ButtonData *)button->data;
    if (!data)
        return NULL;

    return data->text;
}

void button_auto_size(Widget *button)
{
    if (!button || button->type != WIDGET_TYPE_BUTTON)
        return;

    ButtonData *data = (ButtonData *)button->data;
    if (!data || !data->font || !data->text)
        return;

    int text_width = measureTextWidth(data->text, data->font);
    int text_height = getFontHeight(data->font);

    int total_padding = data->padding * 2;
    int total_border = data->border_thickness * 2;

    button->width = text_width + total_padding + total_border;
    button->height = text_height + total_padding + total_border;
}

Widget *button_create_auto(int x, int y, const char *text, const bdf_font_t *font)
{
    Widget *button = button_create(x, y, 100, 40, text);
    if (!button)
        return NULL;

    button_set_font(button, font);

    button_auto_size(button);

    return button;
}
