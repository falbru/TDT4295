#include "game.h"
#include "game_page.h"
#include "menu_page.h"
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

    Widget *root_container;
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

    g_game.root_container = container_create(0, 0, config->window_width, config->window_height, LAYOUT_TYPE_NONE);
    if (!g_game.root_container)
    {
        return false;
    }

    g_game.menu_container = menu_page_init(config);
    if (!g_game.menu_container)
    {
        game_cleanup();
        return false;
    }
    container_add_child(g_game.root_container, g_game.menu_container);

    g_game.game_container = game_page_init(config);
    if (!g_game.game_container)
    {
        game_cleanup();
        return false;
    }
    container_add_child(g_game.root_container, g_game.game_container);

    g_game.canvas = game_page_get_canvas();

    g_game.initialized = true;

    return true;
}

void game_cleanup(void)
{
    menu_page_cleanup();
    game_page_cleanup();

    if (g_game.root_container)
    {
        widget_destroy(g_game.root_container);
        free(g_game.root_container);
        g_game.root_container = NULL;
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

void game_update(float delta_time)
{
    if (!g_game.initialized)
        return;

    if (g_game.state == GAME_STATE_MENU)
    {
        menu_page_update(delta_time);
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

        widget_handle_dirty(g_game.root_container, framebuffer);

        framebuffer_clear_dirty_rects(framebuffer, BACKGROUND_COLOR);

        widget_render(g_game.root_container, framebuffer);

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

void game_on_guess(Widget *widget, void *user_data)
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

void game_on_retry(Widget *widget, void *user_data)
{
    game_start_new_round();
}

void game_on_play(Widget *widget, void *user_data)
{
    widget_set_visible(g_game.menu_container, false);
    widget_set_visible(g_game.game_container, true);

    game_page_reset_round();
    game_start_new_round();
}

void game_on_menu(Widget *widget, void *user_data)
{
    widget_set_visible(g_game.game_container, false);
    widget_set_visible(g_game.menu_container, true);

    g_game.state = GAME_STATE_MENU;
}

void game_on_skip(Widget *widget, void *user_data)
{
    game_start_new_round();
}
