#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "framebuffer.h"
#include <stdint.h>

void renderText(const char *text, uint8_t color, int x, int y, Framebuffer *framebuffer);

#endif
