#ifndef FRAMEBUFFER_H_INCLUDED
#define FRAMEBUFFER_H_INCLUDED

#include <stdint.h>
typedef struct
{
    uint8_t *pixels;
    int width;
    int height;
} Framebuffer;

#endif
