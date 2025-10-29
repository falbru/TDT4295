#ifndef FONT_TYPES_H
#define FONT_TYPES_H

typedef struct {
  int encoding;
  int width;
  int bbx_width;
  int bbx_height;
  int bbx_xoff;
  int bbx_yoff;
  const unsigned int *bitmap;
} bdf_char_t;

typedef struct {
  const bdf_char_t *chars;
  int char_count;
} bdf_font_t;

#endif
