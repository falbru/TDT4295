#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include "color.h"
#include "widgets/widget.h"

typedef struct {
  uint8_t *pixels;
  int brush_size;
  Color background_color;
  Color brush_color;
  Color border_color;
  int border_thickness;
} CanvasData;

Widget *canvas_create(int x, int y, int width, int height);

void canvas_set_brush_size(Widget *canvas, int size);
void canvas_set_brush_color(Widget *canvas, Color color);
void canvas_set_background_color(Widget *canvas, Color color);
void canvas_set_border(Widget *canvas, Color color, int thickness);

void canvas_draw_at(Widget *canvas, int x, int y);
void canvas_clear(Widget *canvas);

#endif
