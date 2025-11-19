#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include <stdint.h>

typedef struct {
    uint8_t a;  // Alpha
    uint8_t r;  // Red
    uint8_t g;  // Green
    uint8_t b;  // Blue
} Color;

// Predefined ARGB colors
#define COLOR_BLACK   ((Color){0xFF, 0x00, 0x00, 0x00})
#define COLOR_WHITE   ((Color){0xFF, 0xFF, 0xFF, 0xFF})
#define COLOR_GRAY_25 ((Color){0xFF, 0x40, 0x40, 0x40})
#define COLOR_GRAY_50 ((Color){0xFF, 0x80, 0x80, 0x80})
#define COLOR_GRAY_75 ((Color){0xFF, 0xC0, 0xC0, 0xC0})

#define COLOR_TRANSPARENT ((Color){0, 0, 0, 0})
#define COLOR_YELLOW COLOR_RGB(255, 209, 57)

// Helper macros for creating ARGB colors
#define COLOR_ARGB(a, r, g, b) ((Color){a, r, g, b})
#define COLOR_RGB(r, g, b) COLOR_ARGB(0xFF, r, g, b)

#define COLOR_COMPARE(c1, c2) (c1.a == c2.a && c1.r == c2.r && c1.g == c2.g && c1.b == c2.b)

// Extract components from ARGB color
#define COLOR_GET_A(color) ((color).a)
#define COLOR_GET_R(color) ((color).r)
#define COLOR_GET_G(color) ((color).g)
#define COLOR_GET_B(color) ((color).b)

int compare_colors(Color* a, Color* b);

#endif
