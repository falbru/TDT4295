#include "color.h"
#include "unity.h"
#include "widgets/button.h"
#include "widgets/widget.h"
#include <stdlib.h>

static Widget *button;

void setUp(void)
{
    button = button_create(10, 20, 100, 50, "Test");
}

void tearDown(void)
{
    if (button)
    {
        widget_destroy(button);
        free(button);
        button = NULL;
    }
}

void test_button_dirty_on_creation(void)
{
    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_text_marks_dirty(void)
{
    button->dirty = false;

    button_set_text(button, "New Text");

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_text_same_text_marks_dirty(void)
{
    button->dirty = false;

    button_set_text(button, "Test");

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_text_null_marks_dirty(void)
{
    button->dirty = false;

    button_set_text(button, NULL);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_padding_marks_dirty(void)
{
    button->dirty = false;

    button_set_padding(button, 15);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_padding_same_value_does_not_mark_dirty(void)
{
    ButtonData *data = (ButtonData *)button->data;
    int current_padding = data->padding;

    button->dirty = false;

    button_set_padding(button, current_padding);

    TEST_ASSERT_FALSE(button->dirty);
}

void test_button_set_background_color_marks_dirty(void)
{
    button->dirty = false;

    button_set_background_color(button, COLOR_BLACK);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_background_color_same_color_does_not_mark_dirty(void)
{
    button_set_background_color(button, COLOR_RED);
    button->dirty = false;

    button_set_background_color(button, COLOR_RED);

    TEST_ASSERT_FALSE(button->dirty);
}

void test_button_set_text_color_marks_dirty(void)
{
    button->dirty = false;

    button_set_text_color(button, COLOR_WHITE);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_text_color_same_color_does_not_mark_dirty(void)
{
    button_set_text_color(button, COLOR_BLUE);
    button->dirty = false;

    button_set_text_color(button, COLOR_BLUE);

    TEST_ASSERT_FALSE(button->dirty);
}

void test_button_set_border_marks_dirty(void)
{
    button->dirty = false;

    button_set_border(button, COLOR_RED, 3);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_border_same_values_does_not_mark_dirty(void)
{
    button_set_border(button, COLOR_GREEN, 5);
    button->dirty = false;

    button_set_border(button, COLOR_GREEN, 5);

    TEST_ASSERT_FALSE(button->dirty);
}

void test_button_set_border_different_color_marks_dirty(void)
{
    button_set_border(button, COLOR_GREEN, 5);
    button->dirty = false;

    button_set_border(button, COLOR_RED, 5);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_border_different_thickness_marks_dirty(void)
{
    button_set_border(button, COLOR_GREEN, 5);
    button->dirty = false;

    button_set_border(button, COLOR_GREEN, 3);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_font_marks_dirty(void)
{
    const bdf_font_t *mock_font = (const bdf_font_t *)0x12345678;
    button->dirty = false;

    button_set_font(button, mock_font);

    TEST_ASSERT_TRUE(button->dirty);
}

void test_button_set_on_click_does_not_mark_dirty(void)
{
    button->dirty = false;

    button_set_on_click(button, NULL, NULL);

    TEST_ASSERT_FALSE(button->dirty);
}

void test_button_auto_size_returns_early_without_font(void)
{
    button->dirty = false;

    button_auto_size(button);

    TEST_ASSERT_FALSE(button->dirty);
}

void test_button_dirty_propagates_to_parent(void)
{
    Widget *parent = (Widget *)malloc(sizeof(Widget));
    widget_init(parent, WIDGET_TYPE_CONTAINER, 0, 0, 200, 200);

    button->parent = parent;
    parent->dirty = false;
    button->dirty = false;

    button_set_text(button, "Propagate");

    TEST_ASSERT_TRUE(button->dirty);
    TEST_ASSERT_TRUE(parent->dirty);

    free(parent);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_button_dirty_on_creation);
    RUN_TEST(test_button_set_text_marks_dirty);
    RUN_TEST(test_button_set_text_same_text_marks_dirty);
    RUN_TEST(test_button_set_text_null_marks_dirty);
    RUN_TEST(test_button_set_padding_marks_dirty);
    RUN_TEST(test_button_set_padding_same_value_does_not_mark_dirty);
    RUN_TEST(test_button_set_background_color_marks_dirty);
    RUN_TEST(test_button_set_background_color_same_color_does_not_mark_dirty);
    RUN_TEST(test_button_set_text_color_marks_dirty);
    RUN_TEST(test_button_set_text_color_same_color_does_not_mark_dirty);
    RUN_TEST(test_button_set_border_marks_dirty);
    RUN_TEST(test_button_set_border_same_values_does_not_mark_dirty);
    RUN_TEST(test_button_set_border_different_color_marks_dirty);
    RUN_TEST(test_button_set_border_different_thickness_marks_dirty);
    RUN_TEST(test_button_set_font_marks_dirty);
    RUN_TEST(test_button_set_on_click_does_not_mark_dirty);
    RUN_TEST(test_button_auto_size_returns_early_without_font);
    RUN_TEST(test_button_dirty_propagates_to_parent);

    return UNITY_END();
}
