#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include "widget.h"
#include "../primitives/image.h"

typedef struct {
    const Image *image;
    Color border_color;
    int border_thickness;
} ImageWidgetData;

Widget *image_widget_create(int x, int y, const Image *image);
void image_widget_set_border(Widget *image_widget, Color color, int thickness);

#endif // IMAGE_WIDGET_H
