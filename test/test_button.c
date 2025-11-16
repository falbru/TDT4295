#include "color.h"
#include "unity.h"
#include "widgets/button.h"
#include "widgets/widget.h"
#include <stdlib.h>
#include <string.h>

// Helper macro to compare Color structs component by component
#define TEST_ASSERT_EQUAL_COLOR(expected, actual) \
    do { \
        TEST_ASSERT_EQUAL_UINT8((expected).a, (actual).a); \
        TEST_ASSERT_EQUAL_UINT8((expected).r, (actual).r); \
        TEST_ASSERT_EQUAL_UINT8((expected).g, (actual).g); \
        TEST_ASSERT_EQUAL_UINT8((expected).b, (actual).b); \
    } while(0)

static Widget *button;

void setUp(void)
{
    button = button_create(10, 20, 100, 50, "Test Button");
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

void test_button_creation(void)
{
    TEST_ASSERT_NOT_NULL(button);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_BUTTON, button->type);
    TEST_ASSERT_EQUAL_INT(10, button->x);
    TEST_ASSERT_EQUAL_INT(20, button->y);
    TEST_ASSERT_EQUAL_INT(100, button->width);
    TEST_ASSERT_EQUAL_INT(50, button->height);
    TEST_ASSERT_TRUE(button->visible);
    TEST_ASSERT_TRUE(button->enabled);
    TEST_ASSERT_NOT_NULL(button->data);
}

void test_button_creation_with_null_text(void)
{
    Widget *null_text_button = button_create(0, 0, 100, 50, NULL);
    TEST_ASSERT_NOT_NULL(null_text_button);
    TEST_ASSERT_NULL(button_get_text(null_text_button));

    widget_destroy(null_text_button);
    free(null_text_button);
}

void test_button_initial_text(void)
{
    const char *text = button_get_text(button);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_EQUAL_STRING("Test Button", text);
}

void test_button_set_text(void)
{
    button_set_text(button, "New Text");

    const char *text = button_get_text(button);
    TEST_ASSERT_NOT_NULL(text);
    TEST_ASSERT_EQUAL_STRING("New Text", text);
}

void test_button_set_text_to_null(void)
{
    button_set_text(button, NULL);

    const char *text = button_get_text(button);
    TEST_ASSERT_NULL(text);
}

void test_button_set_text_multiple_times(void)
{
    button_set_text(button, "First");
    TEST_ASSERT_EQUAL_STRING("First", button_get_text(button));

    button_set_text(button, "Second");
    TEST_ASSERT_EQUAL_STRING("Second", button_get_text(button));

    button_set_text(button, "Third");
    TEST_ASSERT_EQUAL_STRING("Third", button_get_text(button));
}

void test_button_set_padding(void)
{
    button_set_padding(button, 16);

    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_INT(16, data->padding);
}

void test_button_default_padding(void)
{
    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_INT(8, data->padding);
}

void test_button_set_background_color(void)
{
    button_set_background_color(button, COLOR_WHITE);

    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_WHITE, data->background_color);
}

void test_button_default_background_color(void)
{
    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_GRAY_75, data->background_color);
}

void test_button_set_text_color(void)
{
    button_set_text_color(button, COLOR_WHITE);

    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_WHITE, data->text_color);
}

void test_button_default_text_color(void)
{
    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_BLACK, data->text_color);
}

void test_button_set_border(void)
{
    button_set_border(button, COLOR_GRAY_50, 3);

    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_GRAY_50, data->border_color);
    TEST_ASSERT_EQUAL_INT(3, data->border_thickness);
}

void test_button_default_border(void)
{
    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_COLOR(COLOR_BLACK, data->border_color);
    TEST_ASSERT_EQUAL_INT(1, data->border_thickness);
}

void test_button_set_border_no_border(void)
{
    button_set_border(button, COLOR_BLACK, 0);

    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_INT(0, data->border_thickness);
}

void test_button_set_font(void)
{
    const bdf_font_t *mock_font = (const bdf_font_t *)0x12345678;

    button_set_font(button, mock_font);

    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_EQUAL_PTR(mock_font, data->font);
}

void test_button_default_font(void)
{
    ButtonData *data = (ButtonData *)button->data;
    TEST_ASSERT_NULL(data->font);
}

static bool click_callback_called = false;
static Widget *click_callback_widget = NULL;
static void *click_callback_user_data = NULL;

static void test_click_callback(Widget *widget, void *user_data)
{
    click_callback_called = true;
    click_callback_widget = widget;
    click_callback_user_data = user_data;
}

void test_button_set_on_click(void)
{
    int test_data = 42;

    click_callback_called = false;
    click_callback_widget = NULL;
    click_callback_user_data = NULL;

    button_set_on_click(button, test_click_callback, &test_data);

    TEST_ASSERT_EQUAL_PTR(test_click_callback, button->on_click);
    TEST_ASSERT_EQUAL_PTR(&test_data, button->user_data);

    if (button->on_click)
    {
        button->on_click(button, button->user_data);
    }

    TEST_ASSERT_TRUE(click_callback_called);
    TEST_ASSERT_EQUAL_PTR(button, click_callback_widget);
    TEST_ASSERT_EQUAL_PTR(&test_data, click_callback_user_data);
}

void test_button_set_text_with_invalid_widget_type(void)
{
    Widget *label = (Widget *)malloc(sizeof(Widget));
    widget_init(label, WIDGET_TYPE_LABEL, 0, 0, 100, 50);

    button_set_text(label, "This should not work");

    free(label);
}

void test_button_get_text_with_null_button(void)
{
    const char *text = button_get_text(NULL);
    TEST_ASSERT_NULL(text);
}

void test_button_set_padding_with_null_button(void)
{
    button_set_padding(NULL, 10);
}

void test_button_has_render_callback(void)
{
    TEST_ASSERT_NOT_NULL(button->render);
}

void test_button_has_destroy_callback(void)
{
    TEST_ASSERT_NOT_NULL(button->destroy);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_button_creation);
    RUN_TEST(test_button_creation_with_null_text);
    RUN_TEST(test_button_initial_text);
    RUN_TEST(test_button_set_text);
    RUN_TEST(test_button_set_text_to_null);
    RUN_TEST(test_button_set_text_multiple_times);
    RUN_TEST(test_button_set_padding);
    RUN_TEST(test_button_default_padding);
    RUN_TEST(test_button_set_background_color);
    RUN_TEST(test_button_default_background_color);
    RUN_TEST(test_button_set_text_color);
    RUN_TEST(test_button_default_text_color);
    RUN_TEST(test_button_set_border);
    RUN_TEST(test_button_default_border);
    RUN_TEST(test_button_set_border_no_border);
    RUN_TEST(test_button_set_font);
    RUN_TEST(test_button_default_font);
    RUN_TEST(test_button_set_on_click);
    RUN_TEST(test_button_set_text_with_invalid_widget_type);
    RUN_TEST(test_button_get_text_with_null_button);
    RUN_TEST(test_button_set_padding_with_null_button);
    RUN_TEST(test_button_has_render_callback);
    RUN_TEST(test_button_has_destroy_callback);

    return UNITY_END();
}
