#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED

#include "color.h"

#define MAX_DIRTY_RECTS 32

typedef struct
{
    int x;
    int y;
    int width;
    int height;
} DirtyRect;

typedef struct
{
    Color *pixels;
    int width;
    int height;

    DirtyRect dirty_rects[MAX_DIRTY_RECTS];
    int dirty_rect_count;
} Framebuffer;

// #define FRAMEBUFFER_SET_PIXEL(fb, x, y, color) ((fb)->pixels[((fb)->height - 1 - (y)) * (fb)->width + ((fb)->width - 1 - (x))] = (color))
#define FRAMEBUFFER_SET_PIXEL(fb, x, y, color) ((fb)->pixels[(y) * (fb)->width + (x)] = (color))

void framebuffer_clear(Framebuffer *framebuffer, Color clear_color);
void framebuffer_clear_dirty_rects(Framebuffer *framebuffer, Color clear_color);

#endif
