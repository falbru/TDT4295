#include "game.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

static const char *test_prompts[] = {"cat", "dog", "house", "car", "tree"};
static GameConfig test_config;
static bool guess_callback_called = false;
static uint8_t last_canvas_data[28 * 28];

void test_guess_callback(const uint8_t *canvas_28x28, void *user_data)
{
    guess_callback_called = true;
    memcpy(last_canvas_data, canvas_28x28, 28 * 28);
}

void setUp(void)
{
    test_config.drawing_prompts = test_prompts;
    test_config.num_prompts = 5;
    test_config.window_width = 480;
    test_config.window_height = 320;
    test_config.canvas_width = 224;
    test_config.canvas_height = 224;
    test_config.guess_callback = test_guess_callback;
    test_config.callback_user_data = NULL;
    test_config.label_font = NULL;
    test_config.button_font = NULL;
    guess_callback_called = false;
    memset(last_canvas_data, 0, sizeof(last_canvas_data));
}

void tearDown(void)
{
    if (game_is_initialized())
    {
        game_cleanup();
    }
}

void test_game_init_success(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_TRUE(game_is_initialized());
    TEST_ASSERT_EQUAL_INT(GAME_STATE_MENU, game_get_state());
}

void test_game_init_with_null_config(void)
{
    TEST_ASSERT_FALSE(game_init(NULL));
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_init_with_null_prompts(void)
{
    test_config.drawing_prompts = NULL;
    TEST_ASSERT_FALSE(game_init(&test_config));
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_init_with_zero_prompts(void)
{
    test_config.num_prompts = 0;
    TEST_ASSERT_FALSE(game_init(&test_config));
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_cleanup(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_TRUE(game_is_initialized());

    game_cleanup();
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_double_init(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_TRUE(game_is_initialized());

    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_TRUE(game_is_initialized());
}

void test_game_start_new_round(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));

    game_start_new_round();
    TEST_ASSERT_EQUAL_INT(GAME_STATE_PLAYING, game_get_state());
}

void test_game_start_new_round_when_uninitialized(void)
{
    game_start_new_round();
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_update(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));

    game_update(0.016f);
    TEST_ASSERT_TRUE(game_is_initialized());
}

void test_game_update_when_uninitialized(void)
{
    game_update(0.016f);
}

void test_game_send_guess(void)
{
    test_config.num_prompts = 1;
    TEST_ASSERT_TRUE(game_init(&test_config));
    game_start_new_round();

    game_send_guess(0);
    TEST_ASSERT_EQUAL_INT(GAME_STATE_RESULT, game_get_state());
}

void test_game_send_guess_when_not_playing(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));

    int state_before = game_get_state();
    game_send_guess(0);
    TEST_ASSERT_EQUAL_INT(state_before, game_get_state());
}

void test_game_send_guess_when_uninitialized(void)
{
    game_send_guess(0);
}

void test_game_get_canvas_28x28(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));

    uint8_t canvas[28 * 28];
    memset(canvas, 0xFF, sizeof(canvas));

    game_get_canvas_28x28(canvas);

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

void test_game_get_canvas_28x28_with_null_buffer(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    game_get_canvas_28x28(NULL);
}

void test_game_get_canvas_28x28_when_uninitialized(void)
{
    uint8_t canvas[28 * 28];
    game_get_canvas_28x28(canvas);
}

void test_game_set_random_seed(void)
{
    game_set_random_seed(42);
}

void test_game_get_state(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_EQUAL_INT(GAME_STATE_MENU, game_get_state());

    game_start_new_round();
    TEST_ASSERT_EQUAL_INT(GAME_STATE_PLAYING, game_get_state());
}

void test_game_handle_mouse_down_in_menu(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_EQUAL_INT(GAME_STATE_MENU, game_get_state());

    game_handle_mouse_down(240, 160);
}

void test_game_handle_mouse_down_when_uninitialized(void)
{
    TEST_ASSERT_FALSE(game_handle_mouse_down(100, 100));
}

void test_game_handle_mouse_up_when_uninitialized(void)
{
    TEST_ASSERT_FALSE(game_handle_mouse_up(100, 100));
}

void test_game_handle_mouse_move_when_uninitialized(void)
{
    TEST_ASSERT_FALSE(game_handle_mouse_move(100, 100));
}

void test_game_on_guess(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    game_start_new_round();

    TEST_ASSERT_FALSE(guess_callback_called);
    game_on_guess(NULL, NULL);
    TEST_ASSERT_TRUE(guess_callback_called);
}

void test_game_on_retry(void)
{
    test_config.num_prompts = 1;
    TEST_ASSERT_TRUE(game_init(&test_config));
    game_start_new_round();
    game_send_guess(0);
    TEST_ASSERT_EQUAL_INT(GAME_STATE_RESULT, game_get_state());

    game_on_retry(NULL, NULL);
    TEST_ASSERT_EQUAL_INT(GAME_STATE_PLAYING, game_get_state());
}

void test_game_on_play(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    TEST_ASSERT_EQUAL_INT(GAME_STATE_MENU, game_get_state());

    game_on_play(NULL, NULL);
    TEST_ASSERT_EQUAL_INT(GAME_STATE_PLAYING, game_get_state());
}

void test_game_on_menu(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    game_start_new_round();
    TEST_ASSERT_EQUAL_INT(GAME_STATE_PLAYING, game_get_state());

    game_on_menu(NULL, NULL);
    TEST_ASSERT_EQUAL_INT(GAME_STATE_MENU, game_get_state());
}

void test_game_on_skip(void)
{
    TEST_ASSERT_TRUE(game_init(&test_config));
    game_start_new_round();

    game_on_skip(NULL, NULL);
    TEST_ASSERT_EQUAL_INT(GAME_STATE_PLAYING, game_get_state());
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_game_init_success);
    RUN_TEST(test_game_init_with_null_config);
    RUN_TEST(test_game_init_with_null_prompts);
    RUN_TEST(test_game_init_with_zero_prompts);
    RUN_TEST(test_game_cleanup);
    RUN_TEST(test_game_double_init);
    RUN_TEST(test_game_start_new_round);
    RUN_TEST(test_game_start_new_round_when_uninitialized);
    RUN_TEST(test_game_update);
    RUN_TEST(test_game_update_when_uninitialized);
    RUN_TEST(test_game_send_guess);
    RUN_TEST(test_game_send_guess_when_not_playing);
    RUN_TEST(test_game_send_guess_when_uninitialized);
    RUN_TEST(test_game_get_canvas_28x28);
    RUN_TEST(test_game_get_canvas_28x28_with_null_buffer);
    RUN_TEST(test_game_get_canvas_28x28_when_uninitialized);
    RUN_TEST(test_game_set_random_seed);
    RUN_TEST(test_game_get_state);
    RUN_TEST(test_game_handle_mouse_down_in_menu);
    RUN_TEST(test_game_handle_mouse_down_when_uninitialized);
    RUN_TEST(test_game_handle_mouse_up_when_uninitialized);
    RUN_TEST(test_game_handle_mouse_move_when_uninitialized);
    RUN_TEST(test_game_on_guess);
    RUN_TEST(test_game_on_retry);
    RUN_TEST(test_game_on_play);
    RUN_TEST(test_game_on_menu);
    RUN_TEST(test_game_on_skip);

    return UNITY_END();
}
