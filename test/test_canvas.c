#include "color.h"
#include "framebuffer.h"
#include "unity.h"
#include "widgets/canvas.h"
#include "widgets/widget.h"
#include <stdlib.h>

static Widget *canvas;

void setUp(void)
{
    canvas = canvas_create(10, 20, 100, 80);
}

void tearDown(void)
{
    if (canvas)
    {
        widget_destroy(canvas);
        free(canvas);
        canvas = NULL;
    }
}

void test_canvas_creation(void)
{
    TEST_ASSERT_NOT_NULL(canvas);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_CANVAS, canvas->type);
    TEST_ASSERT_EQUAL_INT(10, canvas->x);
    TEST_ASSERT_EQUAL_INT(20, canvas->y);
    TEST_ASSERT_EQUAL_INT(100, canvas->width);
    TEST_ASSERT_EQUAL_INT(80, canvas->height);
    TEST_ASSERT_TRUE(canvas->visible);
    TEST_ASSERT_TRUE(canvas->enabled);
    TEST_ASSERT_NOT_NULL(canvas->data);
}

void test_canvas_has_pixels(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_NOT_NULL(data->pixels);
}

void test_canvas_pixels_initialized_to_zero(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    int pixel_count = canvas->width * canvas->height;

    for (int i = 0; i < pixel_count; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(0, data->pixels[i]);
    }
}

void test_canvas_default_brush_size(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_INT(3, data->brush_size);
}

void test_canvas_default_background_color(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_UINT8(COLOR_WHITE.a, data->background_color.a);
    TEST_ASSERT_EQUAL_UINT8(COLOR_WHITE.r, data->background_color.r);
    TEST_ASSERT_EQUAL_UINT8(COLOR_WHITE.g, data->background_color.g);
    TEST_ASSERT_EQUAL_UINT8(COLOR_WHITE.b, data->background_color.b);
}

void test_canvas_default_brush_color(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLACK.a, data->brush_color.a);
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLACK.r, data->brush_color.r);
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLACK.g, data->brush_color.g);
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLACK.b, data->brush_color.b);
}

void test_canvas_default_border(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLACK.a, data->border_color.a);
    TEST_ASSERT_EQUAL_INT(2, data->border_thickness);
}

void test_canvas_no_dirty_rect_initially(void)
{
    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_INT(0, data->has_dirty_rect);
}

void test_canvas_set_brush_size(void)
{
    canvas_set_brush_size(canvas, 5);

    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_INT(5, data->brush_size);
}

void test_canvas_set_brush_size_zero_sets_to_one(void)
{
    canvas_set_brush_size(canvas, 0);

    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_INT(1, data->brush_size);
}

void test_canvas_set_brush_size_negative_sets_to_one(void)
{
    canvas_set_brush_size(canvas, -5);

    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_INT(1, data->brush_size);
}

void test_canvas_set_brush_color(void)
{
    canvas_set_brush_color(canvas, COLOR_RED);

    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_UINT8(COLOR_RED.a, data->brush_color.a);
    TEST_ASSERT_EQUAL_UINT8(COLOR_RED.r, data->brush_color.r);
    TEST_ASSERT_EQUAL_UINT8(COLOR_RED.g, data->brush_color.g);
    TEST_ASSERT_EQUAL_UINT8(COLOR_RED.b, data->brush_color.b);
}

void test_canvas_set_background_color(void)
{
    canvas_set_background_color(canvas, COLOR_BLUE);

    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLUE.a, data->background_color.a);
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLUE.r, data->background_color.r);
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLUE.g, data->background_color.g);
    TEST_ASSERT_EQUAL_UINT8(COLOR_BLUE.b, data->background_color.b);
}

void test_canvas_set_border(void)
{
    canvas_set_border(canvas, COLOR_GREEN, 4);

    CanvasData *data = (CanvasData *)canvas->data;
    TEST_ASSERT_EQUAL_UINT8(COLOR_GREEN.a, data->border_color.a);
    TEST_ASSERT_EQUAL_INT(4, data->border_thickness);
}

void test_canvas_draw_at_center_marks_dirty(void)
{
    canvas->dirty = false;

    canvas_draw_at(canvas, 60, 60);

    TEST_ASSERT_TRUE(canvas->dirty);
}

void test_canvas_draw_at_sets_pixels(void)
{
    CanvasData *data = (CanvasData *)canvas->data;

    canvas_draw_at(canvas, 60, 60);

    int canvas_x = 60 - canvas->x;
    int canvas_y = 60 - canvas->y;
    int idx = canvas_y * canvas->width + canvas_x;

    TEST_ASSERT_EQUAL_UINT8(255, data->pixels[idx]);
}

void test_canvas_draw_at_creates_dirty_rect(void)
{
    CanvasData *data = (CanvasData *)canvas->data;

    canvas_draw_at(canvas, 60, 60);

    TEST_ASSERT_EQUAL_INT(1, data->has_dirty_rect);
    TEST_ASSERT_GREATER_THAN(0, data->dirty_width);
    TEST_ASSERT_GREATER_THAN(0, data->dirty_height);
}

void test_canvas_draw_at_outside_bounds_does_not_mark_dirty(void)
{
    canvas->dirty = false;

    canvas_draw_at(canvas, 0, 0);

    TEST_ASSERT_FALSE(canvas->dirty);
}

void test_canvas_draw_at_outside_right_bound(void)
{
    canvas->dirty = false;

    canvas_draw_at(canvas, 200, 50);

    TEST_ASSERT_FALSE(canvas->dirty);
}

void test_canvas_clear_marks_dirty(void)
{
    canvas->dirty = false;

    canvas_clear(canvas);

    TEST_ASSERT_TRUE(canvas->dirty);
}

void test_canvas_clear_resets_all_pixels(void)
{
    CanvasData *data = (CanvasData *)canvas->data;

    canvas_draw_at(canvas, 60, 60);
    canvas_draw_at(canvas, 70, 70);

    canvas_clear(canvas);

    int pixel_count = canvas->width * canvas->height;
    for (int i = 0; i < pixel_count; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(0, data->pixels[i]);
    }
}

void test_canvas_clear_creates_full_dirty_rect(void)
{
    CanvasData *data = (CanvasData *)canvas->data;

    canvas_clear(canvas);

    TEST_ASSERT_EQUAL_INT(1, data->has_dirty_rect);
    TEST_ASSERT_EQUAL_INT(0, data->dirty_x);
    TEST_ASSERT_EQUAL_INT(0, data->dirty_y);
    TEST_ASSERT_EQUAL_INT(canvas->width, data->dirty_width);
    TEST_ASSERT_EQUAL_INT(canvas->height, data->dirty_height);
}

void test_canvas_multiple_draws_expand_dirty_rect(void)
{
    CanvasData *data = (CanvasData *)canvas->data;

    canvas_draw_at(canvas, 30, 30);
    int first_width = data->dirty_width;
    int first_height = data->dirty_height;

    canvas_draw_at(canvas, 80, 80);

    TEST_ASSERT_GREATER_THAN(first_width, data->dirty_width);
    TEST_ASSERT_GREATER_THAN(first_height, data->dirty_height);
}

void test_canvas_dirty_propagates_to_parent(void)
{
    Widget *parent = (Widget *)malloc(sizeof(Widget));
    widget_init(parent, WIDGET_TYPE_CONTAINER, 0, 0, 200, 200);

    canvas->parent = parent;
    parent->dirty = false;
    canvas->dirty = false;

    canvas_draw_at(canvas, 60, 60);

    TEST_ASSERT_TRUE(canvas->dirty);
    TEST_ASSERT_TRUE(parent->dirty);

    free(parent);
}

void test_canvas_has_render_callback(void)
{
    TEST_ASSERT_NOT_NULL(canvas->render);
}

void test_canvas_has_destroy_callback(void)
{
    TEST_ASSERT_NOT_NULL(canvas->destroy);
}

void test_canvas_has_dirty_callback(void)
{
    TEST_ASSERT_NOT_NULL(canvas->on_dirty);
}

void test_canvas_dirty_flag_on_creation(void)
{
    TEST_ASSERT_TRUE(canvas->dirty);
}

void test_canvas_set_brush_size_with_null(void)
{
    canvas_set_brush_size(NULL, 5);
}

void test_canvas_set_brush_color_with_null(void)
{
    canvas_set_brush_color(NULL, COLOR_RED);
}

void test_canvas_set_background_color_with_null(void)
{
    canvas_set_background_color(NULL, COLOR_RED);
}

void test_canvas_set_border_with_null(void)
{
    canvas_set_border(NULL, COLOR_RED, 3);
}

void test_canvas_draw_at_with_null(void)
{
    canvas_draw_at(NULL, 50, 50);
}

void test_canvas_clear_with_null(void)
{
    canvas_clear(NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_canvas_creation);
    RUN_TEST(test_canvas_has_pixels);
    RUN_TEST(test_canvas_pixels_initialized_to_zero);
    RUN_TEST(test_canvas_default_brush_size);
    RUN_TEST(test_canvas_default_background_color);
    RUN_TEST(test_canvas_default_brush_color);
    RUN_TEST(test_canvas_default_border);
    RUN_TEST(test_canvas_no_dirty_rect_initially);
    RUN_TEST(test_canvas_set_brush_size);
    RUN_TEST(test_canvas_set_brush_size_zero_sets_to_one);
    RUN_TEST(test_canvas_set_brush_size_negative_sets_to_one);
    RUN_TEST(test_canvas_set_brush_color);
    RUN_TEST(test_canvas_set_background_color);
    RUN_TEST(test_canvas_set_border);
    RUN_TEST(test_canvas_draw_at_center_marks_dirty);
    RUN_TEST(test_canvas_draw_at_sets_pixels);
    RUN_TEST(test_canvas_draw_at_creates_dirty_rect);
    RUN_TEST(test_canvas_draw_at_outside_bounds_does_not_mark_dirty);
    RUN_TEST(test_canvas_draw_at_outside_right_bound);
    RUN_TEST(test_canvas_clear_marks_dirty);
    RUN_TEST(test_canvas_clear_resets_all_pixels);
    RUN_TEST(test_canvas_clear_creates_full_dirty_rect);
    RUN_TEST(test_canvas_multiple_draws_expand_dirty_rect);
    RUN_TEST(test_canvas_dirty_propagates_to_parent);
    RUN_TEST(test_canvas_has_render_callback);
    RUN_TEST(test_canvas_has_destroy_callback);
    RUN_TEST(test_canvas_has_dirty_callback);
    RUN_TEST(test_canvas_dirty_flag_on_creation);
    RUN_TEST(test_canvas_set_brush_size_with_null);
    RUN_TEST(test_canvas_set_brush_color_with_null);
    RUN_TEST(test_canvas_set_background_color_with_null);
    RUN_TEST(test_canvas_set_border_with_null);
    RUN_TEST(test_canvas_draw_at_with_null);
    RUN_TEST(test_canvas_clear_with_null);

    return UNITY_END();
}
