#include "game.h"
#include "color.h"
#include "font_medium.h"
#include "framebuffer.h"
#include "skidaddle.h"
#include "widgets/button.h"
#include "widgets/canvas.h"
#include "widgets/container.h"
#include "widgets/hbox.h"
#include "widgets/image_widget.h"
#include "widgets/label.h"
#include "widgets/vbox.h"
#include "widgets/widget.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include character images for title
#include "s_char.h"
#include "k_char.h"
#include "i_char.h"
#include "d_char.h"
#include "a_char.h"
#include "l_char.h"
#include "e_char.h"

#define DEFAULT_CANVAS_WIDTH 28 * 8
#define DEFAULT_CANVAS_HEIGHT 28 * 8
#define SPACING_SM 2
#define SPACING_MD 4
#define SPACING_LG 8
#define SPACING_XL 16

#define BACKGROUND_COLOR COLOR_RGB(255, 209, 57)

static void str_concat(char *dest, size_t dest_size, const char *src)
{
    size_t dest_len = strlen(dest);
    size_t i = 0;

    while (src[i] != '\0' && (dest_len + i) < (dest_size - 1))
    {
        dest[dest_len + i] = src[i];
        i++;
    }
    dest[dest_len + i] = '\0';
}

static struct
{
    const char **drawing_prompts;
    unsigned int num_prompts;
    game_guess_callback_t guess_callback;
    void *callback_user_data;

    GameState state;
    unsigned int current_prompt_index;
    bool needs_redraw;
    bool is_drawing;
    bool initialized;
    unsigned int score;

    Widget *top_container;
    Widget *menu_container;
    Widget *game_container;

    Widget *button_container;
    Widget *label_prompt;
    Widget *label_result;
    Widget *button_retry;
    Widget *canvas;
    Widget *skidaddle;
    Widget *button_clear;

    Widget *label_title;
    Widget *button_play;
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
    g_game.needs_redraw = true;
    g_game.is_drawing = false;

    const bdf_font_t *label_font = config->label_font ? config->label_font : &font_medium_font;
    const bdf_font_t *button_font = config->button_font ? config->button_font : &font_medium_font;

    unsigned int canvas_width = config->canvas_width > 0 ? config->canvas_width : DEFAULT_CANVAS_WIDTH;
    unsigned int canvas_height = config->canvas_height > 0 ? config->canvas_height : DEFAULT_CANVAS_HEIGHT;

    // Create top-level container
    g_game.top_container = container_create(0, 0, config->window_width, config->window_height, LAYOUT_TYPE_NONE);
    if (!g_game.top_container)
    {
        return false;
    }

    // Create menu container
    g_game.menu_container = vbox_create(0, 0, config->window_width, config->window_height);
    if (!g_game.menu_container)
    {
        game_cleanup();
        return false;
    }
    container_set_padding(g_game.menu_container, SPACING_LG);
    container_set_spacing(g_game.menu_container, SPACING_XL);
    container_set_alignment(g_game.menu_container, ALIGN_CENTER);
    container_set_justify(g_game.menu_container, ALIGN_CENTER);
    container_add_child(g_game.top_container, g_game.menu_container);

    // Create title as hbox with character images
    g_game.label_title = hbox_create(0, 0, config->window_width, 32); // Auto-size based on children
    if (!g_game.label_title)
    {
        game_cleanup();
        return false;
    }
    container_set_spacing(g_game.label_title, 2); // No spacing between letters
    container_set_justify(g_game.label_title, ALIGN_CENTER);

    // Set up floating animation for the title
    container_set_animation(g_game.label_title, ANIMATION_FLOATING);
    container_set_animation_speed(g_game.label_title, 60); // 60 degrees per second

    // Add character images for "SKIDADDLE"
    const Image *title_chars[] = {&s_image, &k_image, &i_image, &d_image, &a_image, &d_image, &d_image, &l_image, &e_image};
    for (int i = 0; i < 9; i++)
    {
        Widget *char_widget = image_widget_create(0, 0, title_chars[i]);
        if (!char_widget)
        {
            game_cleanup();
            return false;
        }
        container_add_child(g_game.label_title, char_widget);
    }

    container_add_child(g_game.menu_container, g_game.label_title);

    g_game.button_play = button_create_auto(0, 0, "Play", button_font);
    if (!g_game.button_play)
    {
        game_cleanup();
        return false;
    }
    button_set_background_color(g_game.button_play, COLOR_WHITE);
    button_set_text_color(g_game.button_play, COLOR_BLACK);
    button_set_border(g_game.button_play, COLOR_BLACK, 2);
    button_set_on_click(g_game.button_play, game_on_play_click, NULL);
    container_add_child(g_game.menu_container, g_game.button_play);

    // Create game container
    g_game.game_container = vbox_create(0, 0, config->window_width, config->window_height);
    if (!g_game.game_container)
    {
        game_cleanup();
        return false;
    }
    container_set_padding(g_game.game_container, SPACING_LG);
    container_set_spacing(g_game.game_container, SPACING_XL);
    container_set_alignment(g_game.game_container, ALIGN_CENTER);
    widget_set_visible(g_game.game_container, false);
    container_add_child(g_game.top_container, g_game.game_container);

    g_game.label_prompt = label_create_auto(0, 0, "Draw: ", label_font);
    if (!g_game.label_prompt)
    {
        game_cleanup();
        return false;
    }
    label_set_color(g_game.label_prompt, COLOR_BLACK);
    container_add_child(g_game.game_container, g_game.label_prompt);

    g_game.canvas = canvas_create(0, 0, canvas_width, canvas_height);
    if (!g_game.canvas)
    {
        game_cleanup();
        return false;
    }
    canvas_set_brush_size(g_game.canvas, 7);
    canvas_set_brush_color(g_game.canvas, COLOR_BLACK);
    canvas_set_background_color(g_game.canvas, COLOR_BLACK);
    canvas_set_border(g_game.canvas, COLOR_BLACK, 1);
    container_add_child(g_game.game_container, g_game.canvas);

    g_game.button_container = hbox_create(0, 0, config->window_width, 40);
    if (!g_game.button_container)
    {
        game_cleanup();
        return false;
    }
    container_set_spacing(g_game.button_container, SPACING_MD);
    container_set_justify(g_game.button_container, ALIGN_CENTER);

    g_game.label_result = label_create_auto(0, 0, "", label_font);
    if (!g_game.label_result)
    {
        game_cleanup();
        return false;
    }
    label_set_color(g_game.label_result, COLOR_BLACK);
    container_add_child(g_game.game_container, g_game.label_result);

    g_game.button_retry = button_create_auto(0, 0, "Play again", button_font);
    if (!g_game.button_retry)
    {
        game_cleanup();
        return false;
    }
    button_set_background_color(g_game.button_retry, COLOR_WHITE);
    button_set_text_color(g_game.button_retry, COLOR_BLACK);
    button_set_border(g_game.button_retry, COLOR_BLACK, 2);
    button_set_on_click(g_game.button_retry, game_on_retry_click, NULL);
    widget_set_visible(g_game.button_retry, false);
    container_add_child(g_game.button_container, g_game.button_retry);
    container_add_child(g_game.game_container, g_game.button_container);

    g_game.initialized = true;

    return true;
}

void game_cleanup(void)
{
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

    g_game.current_prompt_index = rand() % g_game.num_prompts;
    g_game.state = GAME_STATE_PLAYING;

    char prompt_text[128];
    prompt_text[0] = '\0';
    str_concat(prompt_text, sizeof(prompt_text), "Draw a: ");
    str_concat(prompt_text, sizeof(prompt_text), g_game.drawing_prompts[g_game.current_prompt_index]);
    label_set_text(g_game.label_prompt, prompt_text);
    label_auto_size(g_game.label_prompt);
    container_update_layout(g_game.game_container);

    widget_set_visible(g_game.label_prompt, true);
    widget_set_visible(g_game.label_result, false);

    widget_set_visible(g_game.button_retry, false);

    canvas_clear(g_game.canvas);

    g_game.needs_redraw = true;
}

void game_set_prompt(int index)
{
    g_game.current_prompt_index = index;
}

void game_update(float delta_time)
{
    if (!g_game.initialized)
        return;

    // Update animation for the title
    if (g_game.label_title)
    {
        container_update_animation(g_game.label_title, delta_time);
        g_game.needs_redraw = true;
    }
}

void game_send_guess(int guess_index)
{
    if (!g_game.initialized || g_game.state != GAME_STATE_PLAYING)
        return;

    const char *guess = g_game.drawing_prompts[guess_index];
    bool correct = (guess_index == (int)g_game.current_prompt_index);

    char result_text[256];
    result_text[0] = '\0';
    str_concat(result_text, sizeof(result_text), guess);

    label_set_text(g_game.label_result, result_text);
    label_auto_size(g_game.label_result);

    widget_set_visible(g_game.label_result, true);
    widget_set_visible(g_game.label_prompt, true);

    g_game.needs_redraw = true;

    if (correct)
    {
        g_game.score++;
        g_game.state = GAME_STATE_RESULT;
        widget_set_visible(g_game.button_retry, true);
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

        g_game.needs_redraw = false;

        return true;
}

bool game_needs_redraw(void)
{
    return g_game.needs_redraw;
}

void game_request_redraw(void)
{
    g_game.needs_redraw = true;
}

bool game_handle_mouse_down(unsigned int x, unsigned int y)
{
    if (!g_game.initialized)
        return false;

    if (g_game.state == GAME_STATE_MENU)
    {
        widget_handle_click(g_game.menu_container, x, y);
        g_game.needs_redraw = true;
        return true;
    }

    if (g_game.state == GAME_STATE_PLAYING && widget_contains_point(g_game.canvas, x, y))
    {
        g_game.is_drawing = true;
        canvas_draw_at(g_game.canvas, x, y);
        g_game.needs_redraw = true;
        return true;
    }

    widget_handle_click(g_game.game_container, x, y);
    g_game.needs_redraw = true;

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
        g_game.needs_redraw = true;
        return true;
    }

    return false;
}

void game_get_canvas_28x28(uint8_t *output_buffer)
{
    if (!g_game.initialized || !g_game.canvas || !output_buffer)
        return;

    CanvasData *canvas_data = (CanvasData *)g_game.canvas->data;
    if (!canvas_data || !canvas_data->pixels)
        return;

    int canvas_width = g_game.canvas->width;
    int canvas_height = g_game.canvas->height;

    float x_scale = (float)canvas_width / 28.0f;
    float y_scale = (float)canvas_height / 28.0f;

    for (int out_y = 0; out_y < 28; out_y++)
    {
        for (int out_x = 0; out_x < 28; out_x++)
        {
            float center_x = (out_x + 0.5f) * x_scale;
            float center_y = (out_y + 0.5f) * y_scale;

            int start_x = (int)(center_x - x_scale * 0.5f);
            int end_x = (int)(center_x + x_scale * 0.5f);
            int start_y = (int)(center_y - y_scale * 0.5f);
            int end_y = (int)(center_y + y_scale * 0.5f);

            start_x = start_x < 0 ? 0 : start_x;
            end_x = end_x >= canvas_width ? canvas_width - 1 : end_x;
            start_y = start_y < 0 ? 0 : start_y;
            end_y = end_y >= canvas_height ? canvas_height - 1 : end_y;

            int sum = 0;
            int count = 0;

            for (int y = start_y; y <= end_y; y++)
            {
                for (int x = start_x; x <= end_x; x++)
                {
                    sum += canvas_data->pixels[y * canvas_width + x];
                    count++;
                }
            }

            output_buffer[out_y * 28 + out_x] = count > 0 ? (uint8_t)(sum / count) : 0;
        }
    }
}

int game_get_current_prompt_index(void)
{
    return (int)g_game.current_prompt_index;
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
    return g_game.score;
}

void game_on_guess_click(Widget *widget, void *user_data)
{
    if (!g_game.initialized)
        return;

    if (g_game.guess_callback)
    {
        // g_game.state = GAME_STATE_WAITING_FOR_GUESS;

        uint8_t canvas_28x28[28 * 28];
        game_get_canvas_28x28(canvas_28x28);
        g_game.guess_callback(canvas_28x28, g_game.callback_user_data);
    }
    else
    {
        // g_game.state = GAME_STATE_WAITING_FOR_GUESS;
        game_send_guess(rand() % g_game.num_prompts);
    }
}

void game_on_clear_canvas_click(Widget *widget, void *user_data)
{
    if (!g_game.initialized)
        return;

    canvas_clear(g_game.canvas);
    g_game.needs_redraw = true;
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

    // Start a new round
    game_start_new_round();
}
