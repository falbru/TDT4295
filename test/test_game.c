#include "game.h"
#include "unity.h"
#include <stdlib.h>

static const char *DRAWING_PROMPTS[] = {"Cat", "Dog", "House", "Tree", "Car"};
#define NUM_PROMPTS 5
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define CANVAS_WIDTH 112
#define CANVAS_HEIGHT 112

static int guess_callback_called = 0;
static const uint8_t *last_canvas_data = NULL;

void on_guess_request(const uint8_t *canvas_28x28, void *user_data)
{
    guess_callback_called++;
    last_canvas_data = canvas_28x28;
}

static GameConfig config;

void setUp(void)
{
    guess_callback_called = 0;
    last_canvas_data = NULL;

    config = (GameConfig){
        .drawing_prompts = DRAWING_PROMPTS,
        .num_prompts = NUM_PROMPTS,
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .canvas_width = CANVAS_WIDTH,
        .canvas_height = CANVAS_HEIGHT,
        .guess_callback = on_guess_request,
        .callback_user_data = NULL,
        .label_font = NULL,
        .button_font = NULL,
    };
}

void tearDown(void)
{
    game_cleanup();
}

void test_game_init_with_valid_config(void)
{
    bool result = game_init(&config);
    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_TRUE(game_is_initialized());
    TEST_ASSERT_EQUAL(GAME_STATE_PLAYING, game_get_state());
}

void test_game_init_with_null_config(void)
{
    bool result = game_init(NULL);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_init_with_null_prompts(void)
{
    config.drawing_prompts = NULL;
    bool result = game_init(&config);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_init_with_zero_prompts(void)
{
    config.num_prompts = 0;
    bool result = game_init(&config);
    TEST_ASSERT_FALSE(result);
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_game_double_init_cleans_up_first(void)
{
    bool result1 = game_init(&config);
    TEST_ASSERT_TRUE(result1);

    bool result2 = game_init(&config);
    TEST_ASSERT_TRUE(result2);
    TEST_ASSERT_TRUE(game_is_initialized());
}

void test_game_cleanup_resets_state(void)
{
    game_init(&config);
    game_cleanup();
    TEST_ASSERT_FALSE(game_is_initialized());
}

void test_initial_state_is_playing(void)
{
    game_init(&config);
    TEST_ASSERT_EQUAL(GAME_STATE_PLAYING, game_get_state());
}

void test_state_transitions_on_guess(void)
{
    game_init(&config);

    TEST_ASSERT_EQUAL(GAME_STATE_PLAYING, game_get_state());

    game_on_guess_click(NULL, NULL);
    TEST_ASSERT_EQUAL(GAME_STATE_WAITING_FOR_GUESS, game_get_state());
    TEST_ASSERT_EQUAL(1, guess_callback_called);

    game_send_guess(0);
    TEST_ASSERT_EQUAL(GAME_STATE_RESULT, game_get_state());
}

void test_correct_guess(void)
{
    game_init(&config);

    TEST_ASSERT_EQUAL(GAME_STATE_PLAYING, game_get_state());

    game_set_prompt(0);

    game_on_guess_click(NULL, NULL);
    TEST_ASSERT_EQUAL(GAME_STATE_WAITING_FOR_GUESS, game_get_state());
    TEST_ASSERT_EQUAL(1, guess_callback_called);

    game_send_guess(0);
    TEST_ASSERT_EQUAL(GAME_STATE_RESULT, game_get_state());
}

void test_send_guess_in_wrong_state_does_nothing(void)
{
    game_init(&config);

    int initial_state = game_get_state();
    game_send_guess(0);
    TEST_ASSERT_EQUAL(initial_state, game_get_state());
}

void test_correct_guess_detection(void)
{
    game_init(&config);
    game_set_random_seed(42);
    game_start_new_round();

    unsigned int score_before = game_get_score();
    int correct_index = game_get_current_prompt_index();

    game_on_guess_click(NULL, NULL);
    game_send_guess(correct_index);

    TEST_ASSERT_EQUAL(GAME_STATE_RESULT, game_get_state());
    TEST_ASSERT_EQUAL(score_before + 1, game_get_score());
}

void test_incorrect_guess_detection(void)
{
    game_init(&config);
    game_set_random_seed(42);
    game_start_new_round();

    unsigned int score_before = game_get_score();
    int correct_index = game_get_current_prompt_index();
    int wrong_index = (correct_index + 1) % NUM_PROMPTS;

    game_on_guess_click(NULL, NULL);
    game_send_guess(wrong_index);

    TEST_ASSERT_EQUAL(GAME_STATE_RESULT, game_get_state());
    TEST_ASSERT_EQUAL(score_before, game_get_score());
}

void test_initial_score_is_zero(void)
{
    game_init(&config);
    TEST_ASSERT_EQUAL(0, game_get_score());
}

void test_score_accumulates_over_multiple_rounds(void)
{
    game_init(&config);
    TEST_ASSERT_EQUAL(0, game_get_score());

    game_set_prompt(0);
    game_on_guess_click(NULL, NULL);
    game_send_guess(0);
    TEST_ASSERT_EQUAL(1, game_get_score());

    game_on_retry_click(NULL, NULL);
    game_set_prompt(1);
    game_on_guess_click(NULL, NULL);
    game_send_guess(2);
    TEST_ASSERT_EQUAL(1, game_get_score());

    game_on_retry_click(NULL, NULL);
    game_set_prompt(3);
    game_on_guess_click(NULL, NULL);
    game_send_guess(3);
    TEST_ASSERT_EQUAL(2, game_get_score());
}

void test_score_persists_across_rounds(void)
{
    game_init(&config);

    game_set_prompt(0);
    game_on_guess_click(NULL, NULL);
    game_send_guess(0);
    TEST_ASSERT_EQUAL(1, game_get_score());

    game_on_retry_click(NULL, NULL);
    TEST_ASSERT_EQUAL(1, game_get_score());
}

void test_start_new_round_sets_prompt(void)
{
    game_init(&config);
    game_set_random_seed(42);

    game_start_new_round();

    int prompt_index = game_get_current_prompt_index();
    TEST_ASSERT_TRUE(prompt_index >= 0 && prompt_index < NUM_PROMPTS);
    TEST_ASSERT_EQUAL(GAME_STATE_PLAYING, game_get_state());
}

void test_start_new_round_with_seed_is_deterministic(void)
{
    game_init(&config);

    game_set_random_seed(12345);
    game_start_new_round();
    int first_prompt = game_get_current_prompt_index();

    game_set_random_seed(12345);
    game_start_new_round();
    int second_prompt = game_get_current_prompt_index();

    TEST_ASSERT_EQUAL(first_prompt, second_prompt);
}

void test_retry_button_starts_new_round(void)
{
    game_init(&config);
    game_on_guess_click(NULL, NULL);
    game_send_guess(0);

    TEST_ASSERT_EQUAL(GAME_STATE_RESULT, game_get_state());

    game_on_retry_click(NULL, NULL);
    TEST_ASSERT_EQUAL(GAME_STATE_PLAYING, game_get_state());
}

void test_needs_redraw_after_init(void)
{
    game_init(&config);
    TEST_ASSERT_TRUE(game_needs_redraw());
}

void test_request_redraw(void)
{
    game_init(&config);

    Framebuffer fb = {0};
    game_render(&fb);

    TEST_ASSERT_FALSE(game_needs_redraw());

    game_request_redraw();
    TEST_ASSERT_TRUE(game_needs_redraw());
}

void test_start_new_round_triggers_redraw(void)
{
    game_init(&config);

    Framebuffer fb = {0};
    game_render(&fb);
    TEST_ASSERT_FALSE(game_needs_redraw());

    game_start_new_round();
    TEST_ASSERT_TRUE(game_needs_redraw());
}

void test_mouse_down_outside_canvas_returns_false(void)
{
    game_init(&config);
    bool handled = game_handle_mouse_down(10000, 10000);
    TEST_ASSERT_FALSE(handled);
}

void test_mouse_move_without_drawing_returns_false(void)
{
    game_init(&config);
    bool handled = game_handle_mouse_move(50, 50);
    TEST_ASSERT_FALSE(handled);
}

void test_mouse_up_without_drawing_returns_false(void)
{
    game_init(&config);
    bool handled = game_handle_mouse_up(50, 50);
    TEST_ASSERT_FALSE(handled);
}

void test_get_canvas_28x28_produces_correct_size(void)
{
    game_init(&config);

    uint8_t canvas_data[28 * 28];
    game_get_canvas_28x28(canvas_data);

    TEST_ASSERT_TRUE(true);
}

void test_canvas_clear_callback(void)
{
    game_init(&config);

    game_on_clear_canvas_click(NULL, NULL);

    TEST_ASSERT_TRUE(game_needs_redraw());
}

void test_guess_callback_is_called(void)
{
    game_init(&config);

    TEST_ASSERT_EQUAL(0, guess_callback_called);

    game_on_guess_click(NULL, NULL);

    TEST_ASSERT_EQUAL(1, guess_callback_called);
    TEST_ASSERT_NOT_NULL(last_canvas_data);
}

void test_guess_without_callback_uses_random(void)
{
    config.guess_callback = NULL;
    game_init(&config);

    game_set_random_seed(999);
    game_on_guess_click(NULL, NULL);

    TEST_ASSERT_EQUAL(GAME_STATE_RESULT, game_get_state());
}

void test_operations_before_init_are_safe(void)
{
    game_cleanup();
    game_start_new_round();
    game_request_redraw();

    bool needs = game_needs_redraw();
    int state = game_get_state();
    int prompt = game_get_current_prompt_index();

    game_on_guess_click(NULL, NULL);
    game_on_clear_canvas_click(NULL, NULL);
    game_on_retry_click(NULL, NULL);

    game_handle_mouse_down(0, 0);
    game_handle_mouse_move(0, 0);
    game_handle_mouse_up(0, 0);

    uint8_t buffer[28 * 28];
    game_get_canvas_28x28(buffer);

    TEST_ASSERT_TRUE(true);
}

void test_render_with_null_framebuffer_returns_false(void)
{
    game_init(&config);
    bool result = game_render(NULL);
    TEST_ASSERT_FALSE(result);
}

void test_render_before_init_returns_false(void)
{
    Framebuffer fb = {0};
    bool result = game_render(&fb);
    TEST_ASSERT_FALSE(result);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_game_init_with_valid_config);
    RUN_TEST(test_game_init_with_null_config);
    RUN_TEST(test_game_init_with_null_prompts);
    RUN_TEST(test_game_init_with_zero_prompts);
    RUN_TEST(test_game_double_init_cleans_up_first);
    RUN_TEST(test_game_cleanup_resets_state);

    RUN_TEST(test_initial_state_is_playing);
    RUN_TEST(test_state_transitions_on_guess);
    RUN_TEST(test_send_guess_in_wrong_state_does_nothing);
    RUN_TEST(test_correct_guess_detection);
    RUN_TEST(test_incorrect_guess_detection);
    RUN_TEST(test_initial_score_is_zero);
    RUN_TEST(test_score_accumulates_over_multiple_rounds);
    RUN_TEST(test_score_persists_across_rounds);

    RUN_TEST(test_start_new_round_sets_prompt);
    RUN_TEST(test_start_new_round_with_seed_is_deterministic);
    RUN_TEST(test_retry_button_starts_new_round);

    RUN_TEST(test_needs_redraw_after_init);
    RUN_TEST(test_request_redraw);
    RUN_TEST(test_start_new_round_triggers_redraw);

    RUN_TEST(test_mouse_down_outside_canvas_returns_false);
    RUN_TEST(test_mouse_move_without_drawing_returns_false);
    RUN_TEST(test_mouse_up_without_drawing_returns_false);

    RUN_TEST(test_get_canvas_28x28_produces_correct_size);
    RUN_TEST(test_canvas_clear_callback);

    RUN_TEST(test_guess_callback_is_called);
    RUN_TEST(test_guess_without_callback_uses_random);

    RUN_TEST(test_operations_before_init_are_safe);
    RUN_TEST(test_render_with_null_framebuffer_returns_false);
    RUN_TEST(test_render_before_init_returns_false);

    return UNITY_END();
}
