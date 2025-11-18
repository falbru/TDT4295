#include "unity.h"
#include "widgets/widget.h"
#include "framebuffer.h"
#include <stdlib.h>

static Widget *widget;

void setUp(void)
{
    widget = (Widget *)malloc(sizeof(Widget));
}

void tearDown(void)
{
    if (widget)
    {
        widget_destroy(widget);
        free(widget);
        widget = NULL;
    }
}

void test_widget_initialization(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_LABEL, widget->type);
    TEST_ASSERT_EQUAL_INT(10, widget->x);
    TEST_ASSERT_EQUAL_INT(20, widget->y);
    TEST_ASSERT_EQUAL_INT(100, widget->width);
    TEST_ASSERT_EQUAL_INT(50, widget->height);
    TEST_ASSERT_TRUE(widget->visible);
    TEST_ASSERT_TRUE(widget->enabled);
    TEST_ASSERT_NULL(widget->parent);
    TEST_ASSERT_NULL(widget->on_click);
    TEST_ASSERT_NULL(widget->render);
    TEST_ASSERT_NULL(widget->destroy);
    TEST_ASSERT_NULL(widget->user_data);
    TEST_ASSERT_NULL(widget->data);
}

void test_widget_initialization_with_null(void)
{
    widget_init(NULL, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
}

void test_widget_dirty_flag_initially_true(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    TEST_ASSERT_TRUE(widget->dirty);
}

void test_widget_mark_dirty(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_mark_dirty(widget);

    TEST_ASSERT_TRUE(widget->dirty);
}

void test_widget_mark_dirty_with_null(void)
{
    widget_mark_dirty(NULL);
}

void test_widget_mark_dirty_propagates_to_parent(void)
{
    Widget *parent = (Widget *)malloc(sizeof(Widget));
    widget_init(parent, WIDGET_TYPE_CONTAINER, 0, 0, 200, 200);
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    widget->parent = parent;
    parent->dirty = false;
    widget->dirty = false;

    widget_mark_dirty(widget);

    TEST_ASSERT_TRUE(widget->dirty);
    TEST_ASSERT_TRUE(parent->dirty);

    free(parent);
}

void test_widget_set_position(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_set_position(widget, 30, 40);

    TEST_ASSERT_EQUAL_INT(30, widget->x);
    TEST_ASSERT_EQUAL_INT(40, widget->y);
    TEST_ASSERT_TRUE(widget->dirty);
}

void test_widget_set_position_same_position_no_dirty(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_set_position(widget, 10, 20);

    TEST_ASSERT_EQUAL_INT(10, widget->x);
    TEST_ASSERT_EQUAL_INT(20, widget->y);
    TEST_ASSERT_FALSE(widget->dirty);
}

void test_widget_set_position_with_null(void)
{
    widget_set_position(NULL, 10, 20);
}

void test_widget_set_size(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_set_size(widget, 150, 75);

    TEST_ASSERT_EQUAL_INT(150, widget->width);
    TEST_ASSERT_EQUAL_INT(75, widget->height);
    TEST_ASSERT_TRUE(widget->dirty);
}

void test_widget_set_size_same_size_no_dirty(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_set_size(widget, 100, 50);

    TEST_ASSERT_EQUAL_INT(100, widget->width);
    TEST_ASSERT_EQUAL_INT(50, widget->height);
    TEST_ASSERT_FALSE(widget->dirty);
}

void test_widget_set_size_with_null(void)
{
    widget_set_size(NULL, 100, 50);
}

void test_widget_set_visible(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_set_visible(widget, false);

    TEST_ASSERT_FALSE(widget->visible);
    TEST_ASSERT_TRUE(widget->dirty);
}

void test_widget_set_visible_same_state_no_dirty(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->dirty = false;

    widget_set_visible(widget, true);

    TEST_ASSERT_TRUE(widget->visible);
    TEST_ASSERT_FALSE(widget->dirty);
}

void test_widget_set_visible_with_null(void)
{
    widget_set_visible(NULL, false);
}

void test_widget_set_enabled(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    widget_set_enabled(widget, false);

    TEST_ASSERT_FALSE(widget->enabled);
}

void test_widget_set_enabled_with_null(void)
{
    widget_set_enabled(NULL, false);
}

void test_widget_contains_point_inside(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    TEST_ASSERT_TRUE(widget_contains_point(widget, 10, 20));
    TEST_ASSERT_TRUE(widget_contains_point(widget, 50, 40));
    TEST_ASSERT_TRUE(widget_contains_point(widget, 109, 69));
}

void test_widget_contains_point_outside(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    TEST_ASSERT_FALSE(widget_contains_point(widget, 9, 20));
    TEST_ASSERT_FALSE(widget_contains_point(widget, 10, 19));
    TEST_ASSERT_FALSE(widget_contains_point(widget, 110, 20));
    TEST_ASSERT_FALSE(widget_contains_point(widget, 10, 70));
    TEST_ASSERT_FALSE(widget_contains_point(widget, 0, 0));
    TEST_ASSERT_FALSE(widget_contains_point(widget, 200, 200));
}

void test_widget_contains_point_with_null(void)
{
    TEST_ASSERT_FALSE(widget_contains_point(NULL, 10, 20));
}

static bool click_callback_called = false;
static Widget *click_callback_widget = NULL;
static void *click_callback_user_data = NULL;

static void test_click_callback(Widget *w, void *user_data)
{
    click_callback_called = true;
    click_callback_widget = w;
    click_callback_user_data = user_data;
}

void test_widget_handle_click_inside(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    int test_data = 42;
    widget->on_click = test_click_callback;
    widget->user_data = &test_data;

    click_callback_called = false;
    click_callback_widget = NULL;
    click_callback_user_data = NULL;

    widget_handle_click(widget, 50, 40);

    TEST_ASSERT_TRUE(click_callback_called);
    TEST_ASSERT_EQUAL_PTR(widget, click_callback_widget);
    TEST_ASSERT_EQUAL_PTR(&test_data, click_callback_user_data);
}

void test_widget_handle_click_outside(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);

    widget->on_click = test_click_callback;

    click_callback_called = false;

    widget_handle_click(widget, 0, 0);

    TEST_ASSERT_FALSE(click_callback_called);
}

void test_widget_handle_click_disabled(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->enabled = false;
    widget->on_click = test_click_callback;

    click_callback_called = false;

    widget_handle_click(widget, 50, 40);

    TEST_ASSERT_FALSE(click_callback_called);
}

void test_widget_handle_click_invisible(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->visible = false;
    widget->on_click = test_click_callback;

    click_callback_called = false;

    widget_handle_click(widget, 50, 40);

    TEST_ASSERT_FALSE(click_callback_called);
}

void test_widget_handle_click_with_null(void)
{
    widget_handle_click(NULL, 10, 20);
}

void test_widget_handle_click_no_callback(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->on_click = NULL;

    widget_handle_click(widget, 50, 40);
}

static bool render_callback_called = false;

static void test_render_callback(Widget *w, Framebuffer *fb)
{
    render_callback_called = true;
}

void test_widget_render_calls_callback(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->render = test_render_callback;
    widget->dirty = true;

    Framebuffer fb = {0};
    render_callback_called = false;

    widget_render(widget, &fb);

    TEST_ASSERT_TRUE(render_callback_called);
    TEST_ASSERT_FALSE(widget->dirty);
}

void test_widget_render_stores_prev_geometry(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->render = test_render_callback;
    widget->dirty = true;

    Framebuffer fb = {0};

    widget_render(widget, &fb);

    TEST_ASSERT_EQUAL_INT(10, widget->prev_x);
    TEST_ASSERT_EQUAL_INT(20, widget->prev_y);
    TEST_ASSERT_EQUAL_INT(100, widget->prev_width);
    TEST_ASSERT_EQUAL_INT(50, widget->prev_height);
}

void test_widget_render_skips_if_not_dirty(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->render = test_render_callback;
    widget->dirty = false;

    Framebuffer fb = {0};
    render_callback_called = false;

    widget_render(widget, &fb);

    TEST_ASSERT_FALSE(render_callback_called);
}

void test_widget_render_skips_if_not_visible(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->render = test_render_callback;
    widget->dirty = true;
    widget->visible = false;

    Framebuffer fb = {0};
    render_callback_called = false;

    widget_render(widget, &fb);

    TEST_ASSERT_FALSE(render_callback_called);
}

void test_widget_render_with_null_widget(void)
{
    Framebuffer fb = {0};
    widget_render(NULL, &fb);
}

void test_widget_render_with_null_framebuffer(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget_render(widget, NULL);
}

static bool dirty_callback_called = false;

static void test_dirty_callback(Widget *w, Framebuffer *fb)
{
    dirty_callback_called = true;
}

void test_widget_handle_dirty_calls_callback(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->on_dirty = test_dirty_callback;
    widget->dirty = true;

    Framebuffer fb = {0};
    fb.dirty_rect_count = 0;
    dirty_callback_called = false;

    widget_handle_dirty(widget, &fb);

    TEST_ASSERT_TRUE(dirty_callback_called);
}

void test_widget_handle_dirty_without_callback(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->on_dirty = NULL;
    widget->dirty = true;
    widget->prev_x = 5;
    widget->prev_y = 10;
    widget->prev_width = 80;
    widget->prev_height = 40;

    Framebuffer fb = {0};
    fb.dirty_rect_count = 0;

    widget_handle_dirty(widget, &fb);

    TEST_ASSERT_EQUAL_INT(1, fb.dirty_rect_count);
    TEST_ASSERT_EQUAL_INT(5, fb.dirty_rects[0].x);
    TEST_ASSERT_EQUAL_INT(10, fb.dirty_rects[0].y);
    TEST_ASSERT_EQUAL_INT(80, fb.dirty_rects[0].width);
    TEST_ASSERT_EQUAL_INT(40, fb.dirty_rects[0].height);
}

void test_widget_handle_dirty_skips_if_not_dirty(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->on_dirty = test_dirty_callback;
    widget->dirty = false;

    Framebuffer fb = {0};
    dirty_callback_called = false;

    widget_handle_dirty(widget, &fb);

    TEST_ASSERT_FALSE(dirty_callback_called);
}

void test_widget_handle_dirty_with_null_widget(void)
{
    Framebuffer fb = {0};
    widget_handle_dirty(NULL, &fb);
}

static bool destroy_callback_called = false;

static void test_destroy_callback(Widget *w)
{
    destroy_callback_called = true;
}

void test_widget_destroy_calls_callback(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->destroy = test_destroy_callback;

    destroy_callback_called = false;

    widget_destroy(widget);

    TEST_ASSERT_TRUE(destroy_callback_called);
}

void test_widget_destroy_frees_data(void)
{
    widget_init(widget, WIDGET_TYPE_LABEL, 10, 20, 100, 50);
    widget->data = malloc(100);

    widget_destroy(widget);

    TEST_ASSERT_NULL(widget->data);
}

void test_widget_destroy_with_null(void)
{
    widget_destroy(NULL);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_widget_initialization);
    RUN_TEST(test_widget_initialization_with_null);
    RUN_TEST(test_widget_dirty_flag_initially_true);
    RUN_TEST(test_widget_mark_dirty);
    RUN_TEST(test_widget_mark_dirty_with_null);
    RUN_TEST(test_widget_mark_dirty_propagates_to_parent);
    RUN_TEST(test_widget_set_position);
    RUN_TEST(test_widget_set_position_same_position_no_dirty);
    RUN_TEST(test_widget_set_position_with_null);
    RUN_TEST(test_widget_set_size);
    RUN_TEST(test_widget_set_size_same_size_no_dirty);
    RUN_TEST(test_widget_set_size_with_null);
    RUN_TEST(test_widget_set_visible);
    RUN_TEST(test_widget_set_visible_same_state_no_dirty);
    RUN_TEST(test_widget_set_visible_with_null);
    RUN_TEST(test_widget_set_enabled);
    RUN_TEST(test_widget_set_enabled_with_null);
    RUN_TEST(test_widget_contains_point_inside);
    RUN_TEST(test_widget_contains_point_outside);
    RUN_TEST(test_widget_contains_point_with_null);
    RUN_TEST(test_widget_handle_click_inside);
    RUN_TEST(test_widget_handle_click_outside);
    RUN_TEST(test_widget_handle_click_disabled);
    RUN_TEST(test_widget_handle_click_invisible);
    RUN_TEST(test_widget_handle_click_with_null);
    RUN_TEST(test_widget_handle_click_no_callback);
    RUN_TEST(test_widget_render_calls_callback);
    RUN_TEST(test_widget_render_stores_prev_geometry);
    RUN_TEST(test_widget_render_skips_if_not_dirty);
    RUN_TEST(test_widget_render_skips_if_not_visible);
    RUN_TEST(test_widget_render_with_null_widget);
    RUN_TEST(test_widget_render_with_null_framebuffer);
    RUN_TEST(test_widget_handle_dirty_calls_callback);
    RUN_TEST(test_widget_handle_dirty_without_callback);
    RUN_TEST(test_widget_handle_dirty_skips_if_not_dirty);
    RUN_TEST(test_widget_handle_dirty_with_null_widget);
    RUN_TEST(test_widget_destroy_calls_callback);
    RUN_TEST(test_widget_destroy_frees_data);
    RUN_TEST(test_widget_destroy_with_null);

    return UNITY_END();
}
