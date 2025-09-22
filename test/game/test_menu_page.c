#include "game.h"
#include "menu_page.h"
#include "unity.h"
#include "widgets/widget.h"
#include <stdlib.h>

static const char *test_prompts[] = {"cat", "dog", "house"};
static GameConfig test_config;

void setUp(void)
{
    test_config.drawing_prompts = test_prompts;
    test_config.num_prompts = 3;
    test_config.window_width = 480;
    test_config.window_height = 320;
    test_config.canvas_width = 224;
    test_config.canvas_height = 224;
    test_config.guess_callback = NULL;
    test_config.callback_user_data = NULL;
    test_config.label_font = NULL;
    test_config.button_font = NULL;
}

void tearDown(void)
{
    menu_page_cleanup();
}

void test_menu_page_init_success(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_CONTAINER, menu->type);
}

void test_menu_page_init_with_null_config(void)
{
    Widget *menu = menu_page_init(NULL);
    TEST_ASSERT_NULL(menu);
}

void test_menu_page_cleanup(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);

    menu_page_cleanup();
}

void test_menu_page_update(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);

    menu_page_update(0.016f);
    menu_page_update(0.032f);
    menu_page_update(0.1f);
}

void test_menu_page_update_with_zero_delta(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);

    menu_page_update(0.0f);
}

void test_menu_page_update_with_large_delta(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);

    menu_page_update(1.0f);
    menu_page_update(10.0f);
}

void test_menu_page_multiple_updates(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);

    for (int i = 0; i < 100; i++)
    {
        menu_page_update(0.016f);
    }
}

void test_menu_page_with_custom_fonts(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);
}

void test_menu_page_init_with_different_window_sizes(void)
{
    test_config.window_width = 320;
    test_config.window_height = 240;
    Widget *menu1 = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu1);
    menu_page_cleanup();

    test_config.window_width = 640;
    test_config.window_height = 480;
    Widget *menu2 = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu2);
    menu_page_cleanup();

    test_config.window_width = 800;
    test_config.window_height = 600;
    Widget *menu3 = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu3);
}

void test_menu_page_double_cleanup(void)
{
    Widget *menu = menu_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(menu);

    menu_page_cleanup();
    menu_page_cleanup();
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_menu_page_init_success);
    RUN_TEST(test_menu_page_init_with_null_config);
    RUN_TEST(test_menu_page_cleanup);
    RUN_TEST(test_menu_page_update);
    RUN_TEST(test_menu_page_update_with_zero_delta);
    RUN_TEST(test_menu_page_update_with_large_delta);
    RUN_TEST(test_menu_page_multiple_updates);
    RUN_TEST(test_menu_page_with_custom_fonts);
    RUN_TEST(test_menu_page_init_with_different_window_sizes);
    RUN_TEST(test_menu_page_double_cleanup);

    return UNITY_END();
}
