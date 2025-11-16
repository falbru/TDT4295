#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <stdint.h>

typedef uint32_t Color;

// 32-bit ARGB color format: 0xAARRGGBB
#define COLOR_BLACK   ((Color)0xFF000000)
#define COLOR_WHITE   ((Color)0xFFFFFFFF)
#define COLOR_GRAY_25 ((Color)0xFF404040)
#define COLOR_GRAY_50 ((Color)0xFF808080)
#define COLOR_GRAY_75 ((Color)0xFFC0C0C0)

// Helper macros for creating ARGB colors
#define COLOR_ARGB(a, r, g, b) ((Color)(((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xFF, r, g, b)

// Extract components from ARGB color
#define COLOR_GET_A(color) (((color) >> 24) & 0xFF)
#define COLOR_GET_R(color) (((color) >> 16) & 0xFF)
#define COLOR_GET_G(color) (((color) >> 8) & 0xFF)
#define COLOR_GET_B(color) ((color) & 0xFF)

#endif
