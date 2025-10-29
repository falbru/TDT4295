#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "color.h"
#include "framebuffer.h"

void renderFilledRectangle(int x, int y, int width, int height, Color color, Framebuffer *framebuffer);

void renderRectangle(int x, int y, int width, int height, Color color, int thickness, Framebuffer *framebuffer);

#endif
