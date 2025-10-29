#ifndef LABEL_H_INCLUDED
#define LABEL_H_INCLUDED

#include "color.h"
#include "font_types.h"
#include "widgets/widget.h"

typedef struct {
  char *text;
  Color text_color;
  const bdf_font_t *font;
} LabelData;

Widget *label_create(int x, int y, const char *text);
Widget *label_create_auto(int x, int y, const char *text,
                          const bdf_font_t *font);

void label_set_text(Widget *label, const char *text);
void label_set_color(Widget *label, Color color);
void label_set_font(Widget *label, const bdf_font_t *font);

void label_auto_size(Widget *label);

const char *label_get_text(Widget *label);

#endif
