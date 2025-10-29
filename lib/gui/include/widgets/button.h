#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "widgets/widget.h"
#include "color.h"
#include "font_types.h"


typedef struct {
    char *text;
    int padding;
    Color background_color;
    Color text_color;
    Color border_color;
    int border_thickness;
    const bdf_font_t *font;
} ButtonData;


Widget *button_create(int x, int y, int width, int height, const char *text);

Widget *button_create_auto(int x, int y, const char *text, const bdf_font_t *font);

void button_set_text(Widget *button, const char *text);
void button_set_padding(Widget *button, int padding);
void button_set_background_color(Widget *button, Color color);
void button_set_text_color(Widget *button, Color color);
void button_set_border(Widget *button, Color color, int thickness);
void button_set_font(Widget *button, const bdf_font_t *font);
void button_set_on_click(Widget *button, WidgetClickCallback callback, void *user_data);


void button_auto_size(Widget *button);

const char *button_get_text(Widget *button);

#endif
