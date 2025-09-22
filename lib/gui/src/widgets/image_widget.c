#include "widgets/image_widget.h"
#include "primitives/image.h"
#include "primitives/rectangle.h"
#include <stdlib.h>

static void image_widget_render_callback(Widget *widget, Framebuffer *framebuffer);
static void image_widget_destroy_callback(Widget *widget);

Widget *image_widget_create(int x, int y, const Image *image)
{
    Widget *image_widget = (Widget *)malloc(sizeof(Widget));
    if (!image_widget)
        return NULL;

    int width = image ? image->width : 0;
    int height = image ? image->height : 0;

    widget_init(image_widget, WIDGET_TYPE_IMAGE, x, y, width, height);

    ImageWidgetData *data = (ImageWidgetData *)malloc(sizeof(ImageWidgetData));
    if (!data)
    {
        free(image_widget);
        return NULL;
    }

    data->image = image;
    data->border_color = COLOR_BLACK;
    data->border_thickness = 0;

    image_widget->data = data;
    image_widget->render = image_widget_render_callback;
    image_widget->destroy = image_widget_destroy_callback;

    return image_widget;
}

static void image_widget_render_callback(Widget *widget, Framebuffer *framebuffer)
{
    ImageWidgetData *data = (ImageWidgetData *)widget->data;

    if (data->image)
    {
        renderImage(widget->x, widget->y, data->image, framebuffer);
    }

    if (data->border_thickness > 0)
    {
        for (int i = 0; i < data->border_thickness; i++)
        {
            renderRectangle(widget->x - i, widget->y - i, widget->width + 2 * i, widget->height + 2 * i,
                            data->border_color, data->border_thickness, framebuffer);
        }
    }
}

static void image_widget_destroy_callback(Widget *widget)
{
    ImageWidgetData *data = (ImageWidgetData *)widget->data;
    if (data)
    {
        data->image = NULL;
    }
}

void image_widget_set_border(Widget *image_widget, Color color, int thickness)
{
    if (!image_widget || image_widget->type != WIDGET_TYPE_IMAGE)
        return;

    ImageWidgetData *data = (ImageWidgetData *)image_widget->data;
    data->border_color = color;
    data->border_thickness = thickness;

    widget_mark_dirty(image_widget);
}
