#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED

#include "color.h"

#define MAX_DIRTY_RECTS 32

// Used if you want to rotate/flip the screen
#define FRAMEBUFFER_SET_PIXEL(fb, x, y, color) ((fb)->pixels[(y) * (fb)->width + (x)] = (color))
#define FRAMEBUFFER_WIDTH(fb) ((fb)->width)
#define FRAMEBUFFER_HEIGHT(fb) ((fb)->height)

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

void framebuffer_clear(Framebuffer *framebuffer, Color clear_color);
void framebuffer_clear_dirty_rects(Framebuffer *framebuffer, Color clear_color);

#endif
