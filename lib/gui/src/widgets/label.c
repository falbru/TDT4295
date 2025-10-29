#include "widgets/label.h"
#include "primitives/text.h"
#include <stdlib.h>
#include <string.h>

static void label_render_callback(Widget *widget, Framebuffer *framebuffer);
static void label_destroy_callback(Widget *widget);

Widget *label_create(int x, int y, const char *text) {
    Widget *label = (Widget *)malloc(sizeof(Widget));
    if (!label) return NULL;

    widget_init(label, WIDGET_TYPE_LABEL, x, y, 0, 0);

    LabelData *data = (LabelData *)malloc(sizeof(LabelData));
    if (!data) {
        free(label);
        return NULL;
    }

    data->text = text ? strdup(text) : NULL;
    data->text_color = COLOR_WHITE;
    data->font = NULL;

    label->data = data;

    label->render = label_render_callback;
    label->destroy = label_destroy_callback;

    return label;
}

static void label_render_callback(Widget *widget, Framebuffer *framebuffer) {
    if (!widget || !widget->data) return;

    LabelData *data = (LabelData *)widget->data;

    if (data->text && data->font) {
        renderText(data->text, data->text_color, widget->x, widget->y, data->font, framebuffer);
    }
}

static void label_destroy_callback(Widget *widget) {
    if (!widget || !widget->data) return;

    LabelData *data = (LabelData *)widget->data;

    if (data->text) {
        free(data->text);
        data->text = NULL;
    }
}

void label_set_text(Widget *label, const char *text) {
    if (!label || label->type != WIDGET_TYPE_LABEL) return;

    LabelData *data = (LabelData *)label->data;
    if (!data) return;

    if (data->text) {
        free(data->text);
    }

    data->text = text ? strdup(text) : NULL;
}

void label_set_color(Widget *label, Color color) {
    if (!label || label->type != WIDGET_TYPE_LABEL) return;

    LabelData *data = (LabelData *)label->data;
    if (!data) return;

    data->text_color = color;
}

void label_set_font(Widget *label, const bdf_font_t *font) {
    if (!label || label->type != WIDGET_TYPE_LABEL) return;

    LabelData *data = (LabelData *)label->data;
    if (!data) return;

    data->font = font;
}

const char *label_get_text(Widget *label) {
    if (!label || label->type != WIDGET_TYPE_LABEL) return NULL;

    LabelData *data = (LabelData *)label->data;
    if (!data) return NULL;

    return data->text;
}

void label_auto_size(Widget *label) {
    if (!label || label->type != WIDGET_TYPE_LABEL) return;

    LabelData *data = (LabelData *)label->data;
    if (!data || !data->font || !data->text) return;

    int text_width = measureTextWidth(data->text, data->font);
    int text_height = getFontHeight(data->font);

    label->width = text_width;
    label->height = text_height;
}

Widget *label_create_auto(int x, int y, const char *text, const bdf_font_t *font) {
    Widget *label = label_create(x, y, text);
    if (!label) return NULL;

    label_set_font(label, font);

    label_auto_size(label);

    return label;
}
