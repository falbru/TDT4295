#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED

#include "color.h"

typedef struct
{
    Color *pixels;
    int width;
    int height;
} Framebuffer;

#define FRAMEBUFFER_SET_PIXEL(fb, x, y, color) ((fb)->pixels[((fb)->height - 1 - (y)) * (fb)->width + ((fb)->width - 1 - (x))] = (color))

void framebuffer_clear(Framebuffer *framebuffer, Color clear_color);

#endif
