#include <stdio.h> //TODO: remove later
#include "drawing.h"

static uint8_t canvas_buffer[CANVAS_MAX_WIDTH * CANVAS_MAX_HEIGHT]; //TODO: REMOVEEE ON ACTUAL PEODUCT
static bool fpga_buffer[FPGA_MAX_WIDTH * FPGA_MAX_HEIGHT]; //TODO: REMOVEVE

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