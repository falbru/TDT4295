#include <stdlib.h>   // for malloc, free
#include <string.h>   // for memset
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> //TODO: remove later
#include "drawing.h"
#include "brushstroke.h"

//Example values
/*
const int CANVAS_HEIGHT = 252;
const int CANVAS_WIDTH = 252;

const int FPGA_DATA_HEIGHT = 28;
const int FPGA_DATA_WIDTH = 28;

const int KERNEL_HEIGHT = 9;
const int KERNEL_WIDTH = 9;

const int FPGA_DATA_THRESHOLD = 324; // 9*9*8/2 = 324
*/

#define CANVAS_MAX_WIDTH  252
#define CANVAS_MAX_HEIGHT 252
#define FPGA_MAX_WIDTH    28
#define FPGA_MAX_HEIGHT   28
#define KERNEL_WIDTH      9
#define KERNEL_HEIGHT      9
#define FPGA_DATA_THRESHOLD 324

static uint8_t canvas_buffer[CANVAS_MAX_WIDTH * CANVAS_MAX_HEIGHT]; //TODO: REMOVEEE ON ACTUAL PEODUCT
static bool fpga_buffer[FPGA_MAX_WIDTH * FPGA_MAX_HEIGHT]; //TODO: REMOVEVE

void initCanvas(Canvas *canvas, FPGAData *fpga_data) {
    canvas->width = CANVAS_MAX_WIDTH;
    canvas->height = CANVAS_MAX_HEIGHT;
    canvas->pixels = canvas_buffer;
    memset(canvas_buffer, 0, sizeof(canvas_buffer));

    fpga_data->width = FPGA_MAX_WIDTH;
    fpga_data->height = FPGA_MAX_HEIGHT;
    fpga_data->pixels = fpga_buffer;
    memset(fpga_buffer, 0, sizeof(fpga_buffer));
}

void cleanCanvas(Canvas *canvas, FPGAData *fpga_data) {
    // reset sizes
    canvas->width = canvas->height = 0;
    fpga_data->pixels = NULL;
    fpga_data->width = fpga_data->height = 0;
    fpga_data->pixels = NULL;
}

/* 
//Non-static version. Static version is safer.
void initCanvas(int canvas_height, int canvas_width, int fgpa_data_height, int fgpa_data_width, Canvas *canvas, FPGAData *fpga_data) {
    canvas->width = canvas_width;
    canvas->height = canvas_height;
    canvas->pixels = (uint8_t *)malloc(canvas_width * canvas_height * sizeof(uint8_t));
    if (canvas->pixels == NULL) {
        free(canvas->pixels);
        canvas->pixels = NULL;
        canvas->width = canvas->height = 0;
        return;
    }

    memset(canvas->pixels, 0, canvas_width * canvas_height * sizeof(uint8_t));

    fpga_data->width = fpga_data_width;
    fpga_data->height = fpga_data_height;
    fpga_data->pixels = (bool *)malloc(fpga_data_width * fpga_data_height * sizeof(bool));
    if (fpga_data->pixels == NULL) {
        free(canvas->pixels);
        canvas->pixels = NULL;
        fpga_data->width = fpga_data->height = 0;
        return;
    }

    memset(fpga_data->pixels, 0, fpga_data_width * fpga_data_height * sizeof(bool));

}

void cleanCanvas(Canvas *canvas, FPGAData *fpga_data) {

}
*/

void initBrushstroke(int brushstroke_height, int brushstroke_width, Canvas *brushstroke) {
    brushstroke->height = brushstroke_height;
    brushstroke->width = brushstroke_width;
    brushstroke->pixels = brushstroke_diamond;
}

void cleanBrushstroke(Canvas *brushstroke) {
    brushstroke->height = brushstroke->width = 0;
    brushstroke->pixels = NULL;
}

void drawPixel(int x, int y, uint8_t shade, Canvas *canvas) {
    if (x < 0 || x > canvas->width || y < 0 || y > canvas->height) 
        return;
    
    int index = canvas->width * y + x;
    uint16_t sum = canvas->pixels[index] + shade;  // promote to 16-bit to prevent overflow
    if (sum > 255)
        sum = 255;
    canvas->pixels[index] = (uint8_t)sum;
}

void drawBrushStroke(int x, int y, Canvas *brushstroke, Canvas *canvas) {
    // brushstroke is odd numbered sided square.
    int length_from_center = (brushstroke->width - 1) / 2;
    
    for(int i = -1 * length_from_center; i < length_from_center + 1; i++) {
        for(int j = -1 * length_from_center; j < length_from_center + 1; j++) {
            int canvas_x = x + i;
            int canvas_y = y + j;

            if (!(canvas_x < 0 || canvas_x > canvas->width || canvas_y < 0 || canvas_y > canvas->height)) {
                uint8_t shade = brushstroke->pixels[(j + length_from_center) * brushstroke->width + (i + length_from_center)];
                drawPixel(canvas_x, canvas_y, shade, canvas);
            }
            
        }
    }

    //calculate FPGA for correct changed pixels.


}

void calculateFPGAData(int fpga_x, int fpga_y, Canvas *canvas, FPGAData *fpga_data) {
    // since we cannot erase -> we do not need to check a value that is already 1
    if (fpga_data->pixels[fpga_y*FPGA_MAX_WIDTH + fpga_x]) return;

    uint32_t sum = 0;
    for (int i = fpga_x * KERNEL_WIDTH; i < (fpga_x+1) * KERNEL_WIDTH; i++) {
        for (int j = fpga_y * KERNEL_HEIGHT; j < (fpga_y+1) * KERNEL_HEIGHT; j++) {
            sum += canvas->pixels[j * canvas->width + i];
            if (sum >=FPGA_DATA_THRESHOLD) {
                fpga_data->pixels[fpga_y*FPGA_MAX_WIDTH + fpga_x] = true;
                return;
            }
        }
    }
}

int main(void) {
    Canvas canvas;
    FPGAData fpga;
    Canvas brushstroke;

    initCanvas(&canvas, &fpga);
    initBrushstroke(BRUSHSTROKE_HEIGHT, BRUSHSTROKE_WIDTH, &brushstroke);

    printf("Canvas initialized: %dx%d\n", canvas.width, canvas.height);
    printf("FPGA data: %dx%d\n", fpga.width, fpga.height);

    // Make a small test brushstroke

    // Draw the brushstroke in the middle of the canvas
    drawBrushStroke(126, 126, &brushstroke, &canvas);

    // Simulate calculating FPGA data
    calculateFPGAData(5, 5, &canvas, &fpga);

    // Print one example pixel to confirm
    printf("Center pixel intensity: %u\n", canvas.pixels[126 * canvas.width + 126]);
    printf("FPGA[5,5] = %d\n", fpga.pixels[5 * fpga.width + 5]);

    cleanCanvas(&canvas, &fpga);
    cleanBrushstroke(&brushstroke);
    printf("Canvas cleaned.\n");
    return 0;
}