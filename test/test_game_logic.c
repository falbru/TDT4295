#include "../lib/game_logic/include/drawing.h"
#include "../lib/game_logic/include/brushstroke.h"
#include "vendor/Unity/src/unity.h"

#define CANVAS_MAX_WIDTH  252
#define CANVAS_MAX_HEIGHT 252
#define FPGA_MAX_WIDTH    28
#define FPGA_MAX_HEIGHT   28
#define KERNEL_WIDTH      9
#define KERNEL_HEIGHT     9

static uint8_t canvas_buffer[CANVAS_MAX_WIDTH * CANVAS_MAX_HEIGHT];
static bool fpga_buffer[FPGA_MAX_WIDTH * FPGA_MAX_HEIGHT];

static Canvas canvas;
static FPGAData fpga;
static Canvas brushstroke;

// ---- Setup and Teardown ----
void setUp(void)
{
    initCanvas(&canvas, &fpga, canvas_buffer, fpga_buffer);
    initBrushstroke(BRUSHSTROKE_HEIGHT, BRUSHSTROKE_WIDTH, &brushstroke);
}

void tearDown(void)
{
    cleanCanvas(&canvas, &fpga);
    cleanBrushstroke(&brushstroke);
}

// ---- TESTS ----

void test_initCanvas_initializes_buffers_to_zero(void)
{
    // All pixels should be zero after init
    for (int i = 0; i < canvas.width * canvas.height; i++) {
        TEST_ASSERT_EQUAL_UINT8(0, canvas.pixels[i]);
    }

    for (int i = 0; i < fpga.width * fpga.height; i++) {
        TEST_ASSERT_FALSE(fpga.pixels[i]);
    }
}

void test_drawPixel_adds_value_and_clamps_at_255(void)
{
    drawPixel(2, 2, 100, &canvas);
    TEST_ASSERT_EQUAL_UINT8(100, canvas.pixels[2 + 2 * canvas.width]);

    // add again to exceed 255
    drawPixel(2, 2, 200, &canvas);
    TEST_ASSERT_EQUAL_UINT8(255, canvas.pixels[2 + 2 * canvas.width]);
}

void test_drawPixel_does_not_write_out_of_bounds(void)
{
    // reset
    initCanvas(&canvas, &fpga, canvas_buffer, fpga_buffer);
    // Writing outside should not crash or modify memory
    drawPixel(-1, 0, 100, &canvas);
    drawPixel(0, -1, 100, &canvas);
    drawPixel(CANVAS_MAX_WIDTH, 0, 100, &canvas);
    drawPixel(0, CANVAS_MAX_HEIGHT, 100, &canvas);
    // Should still all be zero
    for (int i = 0; i < canvas.width * canvas.height; i++) {
            char msg[64];
            snprintf(msg, sizeof(msg), "Pixel index %d failed (value=%u)", i, canvas.pixels[i]);
            TEST_ASSERT_EQUAL_UINT8_MESSAGE(0, canvas.pixels[i], msg);
    }
}

void test_drawBrushStroke_draws_pattern(void)
{
    drawBrushStroke(5, 5, &brushstroke, &canvas);

    // Check that at least one center pixel got drawn
    uint8_t test_pixel = canvas.pixels[5 * canvas.width + 5];
    TEST_ASSERT_TRUE(test_pixel > 0);
}

void test_calculateFPGAData_sets_true_when_sum_exceeds_threshold(void)
{
    // Manually fill one region with strong pixels
    for (int j = 0; j < KERNEL_HEIGHT; j++) {
        for (int i = 0; i < KERNEL_WIDTH; i++) {
            canvas.pixels[j * canvas.width + i] = 255;
        }
    }

    calculateFPGAData(0, 0, &canvas, &fpga);

    TEST_ASSERT_TRUE(fpga.pixels[0]);
}

void test_cleanCanvas_resets_pointers_and_sizes(void)
{
    cleanCanvas(&canvas, &fpga);
    TEST_ASSERT_NULL(canvas.pixels);
    TEST_ASSERT_EQUAL_INT(0, canvas.width);
    TEST_ASSERT_EQUAL_INT(0, canvas.height);
    TEST_ASSERT_NULL(fpga.pixels);
    TEST_ASSERT_EQUAL_INT(0, fpga.width);
    TEST_ASSERT_EQUAL_INT(0, fpga.height);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_initCanvas_initializes_buffers_to_zero);
    RUN_TEST(test_drawPixel_adds_value_and_clamps_at_255);
    RUN_TEST(test_drawPixel_does_not_write_out_of_bounds);
    RUN_TEST(test_drawBrushStroke_draws_pattern);
    RUN_TEST(test_calculateFPGAData_sets_true_when_sum_exceeds_threshold);
    RUN_TEST(test_cleanCanvas_resets_pointers_and_sizes);
    return UNITY_END();
}
