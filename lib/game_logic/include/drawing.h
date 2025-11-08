#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int width;
    int height;
    uint8_t *pixels;
} Canvas;

typedef struct
{
    int width;
    int height;
    bool *pixels;
} FPGAData;

void initCanvas(Canvas *canvas, FPGAData *fpga_data, uint8_t *canvas_buffer, bool *fpga_buffer);
void cleanCanvas(Canvas *canvas, FPGAData *fpga_data);
void initBrushstroke(int brushstroke_height, int brushstroke_width, Canvas *brushstroke);
void cleanBrushstroke(Canvas *brushstroke);
void drawPixel(int x, int y, uint8_t shade, Canvas *canvas);
void drawBrushStroke(int x, int y, Canvas *brushstroke, Canvas *canvas);
void calculateFPGAData(int fpga_x, int fpga_y, Canvas *canvas, FPGAData *fpga_data);
void calculateAllFPGAData(Canvas *canvas, FPGAData *fpga_data);

#endif