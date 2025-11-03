#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED

#include "color.h"

typedef struct
{
    Color *pixels;
    int width;
    int height;
} Framebuffer;

#endif
