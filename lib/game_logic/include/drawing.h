#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED

typedef struct {
    int width;
    int height;
    uint8_t *pixels;
} Canvas;

typedef struct {
    int width;
    int height;
    bool *pixels;
} FPGAData;

void initCanvas(Canvas *canvas, FPGAData *fpga_data);
void cleanCanvas(Canvas *canvas, FPGAData *fpga_data);
void initBrushstroke(int brushstroke_height, int brushstroke_width, Canvas *brushstroke);


#endif