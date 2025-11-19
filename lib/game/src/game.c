#include "game.h"
#include "game_page.h"
#include "menu.h"
#include "color.h"
#include "framebuffer.h"
#include "widgets/canvas.h"
#include "widgets/container.h"
#include "widgets/widget.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BACKGROUND_COLOR COLOR_RGB(255, 209, 57)

static struct
{
    const char **drawing_prompts;
    unsigned int num_prompts;
    game_guess_callback_t guess_callback;
    void *callback_user_data;

    GameState state;
    bool is_drawing;
    bool initialized;

    Widget *top_container;
    Widget *menu_container;
    Widget *game_container;
    Widget *canvas;
} g_game = {0};

bool game_init(const GameConfig *config)
{
    if (!config || !config->drawing_prompts || config->num_prompts == 0)
    {
        return false;
    }

    if (g_game.initialized)
    {
        game_cleanup();
    }

    g_game.drawing_prompts = config->drawing_prompts;
    g_game.num_prompts = config->num_prompts;
    g_game.guess_callback = config->guess_callback;
    g_game.callback_user_data = config->callback_user_data;
    g_game.state = GAME_STATE_MENU;
    g_game.is_drawing = false;

    // Create top-level container
    g_game.top_container = container_create(0, 0, config->window_width, config->window_height, LAYOUT_TYPE_NONE);
    if (!g_game.top_container)
    {
        return false;
    }

    // Create menu page
    g_game.menu_container = menu_init(config);
    if (!g_game.menu_container)
    {
        game_cleanup();
        return false;
    }
    container_add_child(g_game.top_container, g_game.menu_container);

    // Create game page
    g_game.game_container = game_page_init(config);
    if (!g_game.game_container)
    {
        game_cleanup();
        return false;
    }
    container_add_child(g_game.top_container, g_game.game_container);

    // Store canvas reference for mouse handling
    g_game.canvas = game_page_get_canvas();

    g_game.initialized = true;

    return true;
}

void game_cleanup(void)
{
    menu_cleanup();
    game_page_cleanup();

    if (g_game.top_container)
    {
        widget_destroy(g_game.top_container);
        free(g_game.top_container);
        g_game.top_container = NULL;
    }

    memset(&g_game, 0, sizeof(g_game));
}

void game_start_new_round(void)
{
    if (!g_game.initialized)
        return;

    g_game.state = GAME_STATE_PLAYING;
    game_page_start_new_round();
}

void game_set_prompt(int index)
{
    // This function is no longer used, but kept for API compatibility
}

void game_update(float delta_time)
{
    if (!g_game.initialized)
        return;

    // Update menu animation
    if (g_game.state == GAME_STATE_MENU)
    {
        menu_update(delta_time);
    }
}

void game_send_guess(int guess_index)
{
    if (!g_game.initialized || g_game.state != GAME_STATE_PLAYING)
        return;

    if(game_page_send_guess(guess_index)) {
        g_game.state = GAME_STATE_RESULT;
    }
}

bool game_render(Framebuffer *framebuffer)
{
    if (!g_game.initialized || !framebuffer)
        return false;

    static bool first = true;
        if (first) framebuffer_clear(framebuffer, BACKGROUND_COLOR);
        first = false;

        widget_handle_dirty(g_game.top_container, framebuffer);

        framebuffer_clear_dirty_rects(framebuffer, BACKGROUND_COLOR);

        widget_render(g_game.top_container, framebuffer);

        return true;
}

bool game_handle_mouse_down(unsigned int x, unsigned int y)
{
    if (!g_game.initialized)
        return false;

    if (g_game.state == GAME_STATE_MENU)
    {
        widget_handle_click(g_game.menu_container, x, y);
        return true;
    }

    if (g_game.state == GAME_STATE_PLAYING && widget_contains_point(g_game.canvas, x, y))
    {
        g_game.is_drawing = true;
        canvas_draw_at(g_game.canvas, x, y);
        return true;
    }

    widget_handle_click(g_game.game_container, x, y);

    return false;
}

bool game_handle_mouse_up(unsigned int x, unsigned int y)
{
    if (!g_game.initialized)
        return false;

    if (g_game.is_drawing)
    {
        g_game.is_drawing = false;
        return true;
    }

    return false;
}

bool game_handle_mouse_move(unsigned int x, unsigned int y)
{
    if (!g_game.initialized)
        return false;

    if (g_game.is_drawing && g_game.state == GAME_STATE_PLAYING)
    {
        canvas_draw_at(g_game.canvas, x, y);
        return true;
    }

    return false;
}

void game_get_canvas_28x28(uint8_t *output_buffer)
{
    if (!g_game.initialized || !output_buffer)
        return;

    game_page_get_canvas_28x28(output_buffer);
}

int game_get_current_prompt_index(void)
{
    // This function is no longer accurate, but kept for API compatibility
    return 0;
}

int game_get_state(void)
{
    return (int)g_game.state;
}

bool game_is_initialized(void)
{
    return g_game.initialized;
}

void game_set_random_seed(unsigned int seed)
{
    srand(seed);
}

unsigned int game_get_score(void)
{
    // This function is no longer accurate, but kept for API compatibility
    return 0;
}

void game_on_guess_click(Widget *widget, void *user_data)
{
    if (!g_game.initialized)
        return;

    if (g_game.guess_callback)
    {
        uint8_t canvas_28x28[28 * 28];
        game_get_canvas_28x28(canvas_28x28);
        g_game.guess_callback(canvas_28x28, g_game.callback_user_data);
    }
    else
    {
        game_send_guess(rand() % g_game.num_prompts);
    }
}

void game_on_clear_canvas_click(Widget *widget, void *user_data)
{
    // No longer used, kept for API compatibility
}

void game_on_retry_click(Widget *widget, void *user_data)
{
    game_start_new_round();
}

void game_on_play_click(Widget *widget, void *user_data)
{
    // Hide menu, show game screen
    widget_set_visible(g_game.menu_container, false);
    widget_set_visible(g_game.game_container, true);

    // Reset score and start a new round
    game_page_reset_score();
    game_start_new_round();
}

void game_on_menu_click(Widget *widget, void *user_data)
{
    // Hide game screen, show menu
    widget_set_visible(g_game.game_container, false);
    widget_set_visible(g_game.menu_container, true);

    g_game.state = GAME_STATE_MENU;
}

void game_on_skip_click(Widget *widget, void *user_data)
{
    // Start a new round
    game_start_new_round();
}
