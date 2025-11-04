#include "framebuffer.h"

void framebuffer_clear(Framebuffer *framebuffer, Color clear_color)
{
    for (int i = 0; i < framebuffer->width * framebuffer->height; i++)
    {
        framebuffer->pixels[i] = clear_color;
    }
}
