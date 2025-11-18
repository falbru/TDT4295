#include "color.h"
#include "unity.h"
#include "widgets/label.h"
#include "widgets/widget.h"
#include <stdlib.h>
#include <string.h>

#define TEST_ASSERT_EQUAL_COLOR(expected, actual) \
    do { \
        TEST_ASSERT_EQUAL_UINT8((expected).a, (actual).a); \
        TEST_ASSERT_EQUAL_UINT8((expected).r, (actual).r); \
        TEST_ASSERT_EQUAL_UINT8((expected).g, (actual).g); \
        TEST_ASSERT_EQUAL_UINT8((expected).b, (actual).b); \
    } while(0)

static Widget *label;

void setUp(void)
{
    label = label_create(10, 20, "Test Label");
}

void tearDown(void)
{
    if (label)
    {
        widget_destroy(label);
        free(label);
        label = NULL;
    }
}

void test_label_creation(void)
{
    TEST_ASSERT_NOT_NULL(label);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_LABEL, label->type);
    TEST_ASSERT_EQUAL_INT(10, label->x);
    TEST_ASSERT_EQUAL_INT(20, label->y);
    TEST_ASSERT_EQUAL_INT(0, label->width);
    TEST_ASSERT_EQUAL_INT(0, label->height);
    TEST_ASSERT_TRUE(label->visible);
    TEST_ASSERT_TRUE(label->enabled);
    TEST_ASSERT_NOT_NULL(label->data);
}

void test_label_creation_with_null_text(void)
{
    Widget *null_text_label = label_create(0, 0, NULL);
    TEST_ASSERT_NOT_NULL(null_text_label);
    TEST_ASSERT_NULL(label_get_text(null_text_label));

    widget_destroy(null_text_label);
    free(null_text_label);
}

void test_label_initial_text(void)
{
    const char *text = label_get_text(label);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_EQUAL_STRING("Test Label", text);
}

void test_label_set_text(void)
{
    label_set_text(label, "New Text");

    const char *text = label_get_text(label);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_EQUAL_STRING("New Text", text);
}

void test_label_set_text_to_null(void)
{
    label_set_text(label, NULL);

    const char *text = label_get_text(label);
    TEST_ASSERT_NULL(text);
}

void test_label_set_text_multiple_times(void)
{
    label_set_text(label, "First");
    TEST_ASSERT_EQUAL_STRING("First", label_get_text(label));

    label_set_text(label, "Second");
    TEST_ASSERT_EQUAL_STRING("Second", label_get_text(label));

    label_set_text(label, "Third");
    TEST_ASSERT_EQUAL_STRING("Third", label_get_text(label));
}

void test_label_set_text_marks_dirty(void)
{
    label->dirty = false;

    label_set_text(label, "New Text");

    TEST_ASSERT_TRUE(label->dirty);
}

void test_label_default_text_color(void)
{
    LabelData *data = (LabelData *)label->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_WHITE, data->text_color);
}

void test_label_set_color(void)
{
    label_set_color(label, COLOR_BLACK);

    LabelData *data = (LabelData *)label->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_BLACK, data->text_color);
}

void test_label_set_color_marks_dirty(void)
{
    label->dirty = false;

    label_set_color(label, COLOR_BLACK);

    TEST_ASSERT_TRUE(label->dirty);
}

void test_label_set_font(void)
{
    const bdf_font_t *mock_font = (const bdf_font_t *)0x12345678;

    label_set_font(label, mock_font);

    LabelData *data = (LabelData *)label->data;
    TEST_ASSERT_EQUAL_PTR(mock_font, data->font);
}

void test_label_set_font_marks_dirty(void)
{
    const bdf_font_t *mock_font = (const bdf_font_t *)0x12345678;
    label->dirty = false;

    label_set_font(label, mock_font);

    TEST_ASSERT_TRUE(label->dirty);
}

void test_label_default_font(void)
{
    LabelData *data = (LabelData *)label->data;
    TEST_ASSERT_NULL(data->font);
}

void test_label_set_text_with_invalid_widget_type(void)
{
    Widget *button = (Widget *)malloc(sizeof(Widget));
    widget_init(button, WIDGET_TYPE_BUTTON, 0, 0, 100, 50);

    label_set_text(button, "This should not work");

    free(button);
}

void test_label_get_text_with_null_label(void)
{
    const char *text = label_get_text(NULL);
    TEST_ASSERT_NULL(text);
}

void test_label_set_text_with_null_label(void)
{
    label_set_text(NULL, "Text");
}

void test_label_set_color_with_null_label(void)
{
    label_set_color(NULL, COLOR_BLACK);
}

void test_label_set_font_with_null_label(void)
{
    label_set_font(NULL, NULL);
}

void test_label_has_render_callback(void)
{
    TEST_ASSERT_NOT_NULL(label->render);
}

void test_label_has_destroy_callback(void)
{
    TEST_ASSERT_NOT_NULL(label->destroy);
}

void test_label_dirty_flag_on_creation(void)
{
    TEST_ASSERT_TRUE(label->dirty);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_label_creation);
    RUN_TEST(test_label_creation_with_null_text);
    RUN_TEST(test_label_initial_text);
    RUN_TEST(test_label_set_text);
    RUN_TEST(test_label_set_text_to_null);
    RUN_TEST(test_label_set_text_multiple_times);
    RUN_TEST(test_label_set_text_marks_dirty);
    RUN_TEST(test_label_default_text_color);
    RUN_TEST(test_label_set_color);
    RUN_TEST(test_label_set_color_marks_dirty);
    RUN_TEST(test_label_set_font);
    RUN_TEST(test_label_set_font_marks_dirty);
    RUN_TEST(test_label_default_font);
    RUN_TEST(test_label_set_text_with_invalid_widget_type);
    RUN_TEST(test_label_get_text_with_null_label);
    RUN_TEST(test_label_set_text_with_null_label);
    RUN_TEST(test_label_set_color_with_null_label);
    RUN_TEST(test_label_set_font_with_null_label);
    RUN_TEST(test_label_has_render_callback);
    RUN_TEST(test_label_has_destroy_callback);
    RUN_TEST(test_label_dirty_flag_on_creation);

    return UNITY_END();
}
