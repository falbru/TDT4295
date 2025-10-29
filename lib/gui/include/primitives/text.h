#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "color.h"
#include "font_types.h"
#include "framebuffer.h"

void renderText(const char *text, Color color, int x, int y, const bdf_font_t *font, Framebuffer *framebuffer);

int measureTextWidth(const char *text, const bdf_font_t *font);

int getFontHeight(const bdf_font_t *font);

#endif
