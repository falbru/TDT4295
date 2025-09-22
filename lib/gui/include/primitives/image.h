#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "color.h"
#include "framebuffer.h"
#include <stdint.h>

typedef struct
{
    const Color *data;
    int width;
    int height;
} Image;

void renderImage(int x, int y, const Image *image, Framebuffer *framebuffer);

#endif
