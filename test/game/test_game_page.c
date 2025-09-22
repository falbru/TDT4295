#include "game.h"
#include "game_page.h"
#include "unity.h"
#include "widgets/widget.h"
#include <stdlib.h>
#include <string.h>

static const char *test_prompts[] = {"cat", "dog", "house", "car", "tree"};
static GameConfig test_config;

void setUp(void)
{
    test_config.drawing_prompts = test_prompts;
    test_config.num_prompts = 5;
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
    game_page_cleanup();
}

void test_game_page_init_success(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_CONTAINER, page->type);
}

void test_game_page_init_with_null_config(void)
{
    Widget *page = game_page_init(NULL);
    TEST_ASSERT_NULL(page);
}

void test_game_page_init_with_null_prompts(void)
{
    test_config.drawing_prompts = NULL;
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NULL(page);
}

void test_game_page_init_with_zero_prompts(void)
{
    test_config.num_prompts = 0;
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NULL(page);
}

void test_game_page_cleanup(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_cleanup();
}

void test_game_page_reset_round(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_reset_round();
}

void test_game_page_start_new_round(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_start_new_round();
}

void test_game_page_send_guess_correct(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_start_new_round();

    int result = game_page_send_guess(0);
    TEST_ASSERT_TRUE(result == 0 || result == 1);
}

void test_game_page_send_guess_incorrect(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_start_new_round();

    int result = game_page_send_guess(1);
    TEST_ASSERT_TRUE(result == 0 || result == 1);
}

void test_game_page_get_canvas_28x28(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    uint8_t canvas[28 * 28];
    memset(canvas, 0xFF, sizeof(canvas));

    game_page_get_canvas_28x28(canvas);

    bool all_zeros = true;
    for (int i = 0; i < 28 * 28; i++)
    {
        if (canvas[i] != 0)
        {
            all_zeros = false;
            break;
        }
    }
    TEST_ASSERT_TRUE(all_zeros);
}

void test_game_page_get_canvas_28x28_with_null_buffer(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_get_canvas_28x28(NULL);
}

void test_game_page_get_canvas(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    Widget *canvas = game_page_get_canvas();
    TEST_ASSERT_NOT_NULL(canvas);
    TEST_ASSERT_EQUAL_INT(WIDGET_TYPE_CANVAS, canvas->type);
}

void test_game_page_multiple_rounds(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    for (int i = 0; i < 5; i++)
    {
        game_page_start_new_round();
        game_page_send_guess(i);
    }
}

void test_game_page_reset_after_rounds(void)
{
    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    game_page_start_new_round();
    game_page_send_guess(0);

    game_page_reset_round();
}

void test_game_page_with_default_canvas_size(void)
{
    test_config.canvas_width = 0;
    test_config.canvas_height = 0;

    Widget *page = game_page_init(&test_config);
    TEST_ASSERT_NOT_NULL(page);

    Widget *canvas = game_page_get_canvas();
    TEST_ASSERT_NOT_NULL(canvas);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_game_page_init_success);
    RUN_TEST(test_game_page_init_with_null_config);
    RUN_TEST(test_game_page_init_with_null_prompts);
    RUN_TEST(test_game_page_init_with_zero_prompts);
    RUN_TEST(test_game_page_cleanup);
    RUN_TEST(test_game_page_reset_round);
    RUN_TEST(test_game_page_start_new_round);
    RUN_TEST(test_game_page_send_guess_correct);
    RUN_TEST(test_game_page_send_guess_incorrect);
    RUN_TEST(test_game_page_get_canvas_28x28);
    RUN_TEST(test_game_page_get_canvas_28x28_with_null_buffer);
    RUN_TEST(test_game_page_get_canvas);
    RUN_TEST(test_game_page_multiple_rounds);
    RUN_TEST(test_game_page_reset_after_rounds);
    RUN_TEST(test_game_page_with_default_canvas_size);

    return UNITY_END();
}
