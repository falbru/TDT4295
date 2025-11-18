#include "game.h"
#include "color.h"
#include "font_medium.h"
#include "framebuffer.h"
#include "widgets/button.h"
#include "widgets/canvas.h"
#include "widgets/container.h"
#include "widgets/hbox.h"
#include "widgets/label.h"
#include "widgets/vbox.h"
#include "widgets/widget.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CANVAS_WIDTH 28 * 4
#define DEFAULT_CANVAS_HEIGHT 28 * 4
#define SPACING_SM 2
#define SPACING_MD 4
#define SPACING_LG 8

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

    Widget *root_container;
    Widget *button_container;
    Widget *label_prompt;
    Widget *label_result;
    Widget *canvas;
    Widget *button_guess;
    Widget *button_clear;
    Widget *button_retry;
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
    g_game.state = GAME_STATE_PLAYING;
    g_game.needs_redraw = true;
    g_game.is_drawing = false;

    const bdf_font_t *label_font = config->label_font ? config->label_font : &font_medium_font;
    const bdf_font_t *button_font = config->button_font ? config->button_font : &font_medium_font;

    unsigned int canvas_width = config->canvas_width > 0 ? config->canvas_width : DEFAULT_CANVAS_WIDTH;
    unsigned int canvas_height = config->canvas_height > 0 ? config->canvas_height : DEFAULT_CANVAS_HEIGHT;

    g_game.root_container = vbox_create(0, 0, config->window_width, config->window_height);
    if (!g_game.root_container)
    {
        return false;
    }
    container_set_padding(g_game.root_container, SPACING_LG);
    container_set_spacing(g_game.root_container, SPACING_MD);

    g_game.label_prompt = label_create_auto(0, 0, "Draw: ", label_font);
    if (!g_game.label_prompt)
    {
        game_cleanup();
        return false;
    }
    label_set_color(g_game.label_prompt, COLOR_WHITE);
    container_add_child(g_game.root_container, g_game.label_prompt);

    g_game.canvas = canvas_create(0, 0, canvas_width, canvas_height);
    if (!g_game.canvas)
    {
        game_cleanup();
        return false;
    }
    canvas_set_brush_size(g_game.canvas, 3);
    canvas_set_brush_color(g_game.canvas, COLOR_BLACK);
    canvas_set_background_color(g_game.canvas, COLOR_WHITE);
    canvas_set_border(g_game.canvas, COLOR_WHITE, 2);
    container_add_child(g_game.root_container, g_game.canvas);

    g_game.button_container = hbox_create(0, 0, config->window_width, 40);
    if (!g_game.button_container)
    {
        game_cleanup();
        return false;
    }
    container_set_spacing(g_game.button_container, SPACING_MD);
    container_set_justify(g_game.button_container, ALIGN_CENTER);

    g_game.button_guess = button_create_auto(0, 0, "Guess", button_font);
    if (!g_game.button_guess)
    {
        game_cleanup();
        return false;
    }
    button_set_background_color(g_game.button_guess, COLOR_GRAY_75);
    button_set_text_color(g_game.button_guess, COLOR_BLACK);
    button_set_border(g_game.button_guess, COLOR_WHITE, 2);
    button_set_on_click(g_game.button_guess, game_on_guess_click, NULL);
    container_add_child(g_game.button_container, g_game.button_guess);

    g_game.button_clear = button_create_auto(0, 0, "Clear", button_font);
    if (!g_game.button_clear)
    {
        game_cleanup();
        return false;
    }
    button_set_background_color(g_game.button_clear, COLOR_GRAY_75);
    button_set_text_color(g_game.button_clear, COLOR_BLACK);
    button_set_border(g_game.button_clear, COLOR_WHITE, 2);
    button_set_on_click(g_game.button_clear, game_on_clear_canvas_click, NULL);
    container_add_child(g_game.button_container, g_game.button_clear);

    container_add_child(g_game.root_container, g_game.button_container);

    g_game.button_retry = button_create_auto(0, 0, "Try Again", button_font);
    if (!g_game.button_retry)
    {
        game_cleanup();
        return false;
    }
    button_set_background_color(g_game.button_retry, COLOR_GRAY_75);
    button_set_text_color(g_game.button_retry, COLOR_BLACK);
    button_set_border(g_game.button_retry, COLOR_WHITE, 2);
    button_set_on_click(g_game.button_retry, game_on_retry_click, NULL);
    widget_set_visible(g_game.button_retry, false);
    container_add_child(g_game.root_container, g_game.button_retry);

    g_game.label_result = label_create_auto(0, 0, "", label_font);
    if (!g_game.label_result)
    {
        game_cleanup();
        return false;
    }
    label_set_color(g_game.label_result, COLOR_WHITE);
    container_add_child(g_game.root_container, g_game.label_result);

    container_set_alignment(g_game.root_container, ALIGN_CENTER);

    g_game.initialized = true;

    game_start_new_round();

    return true;
}

void game_cleanup(void)
{
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

    g_game.current_prompt_index = rand() % g_game.num_prompts;
    g_game.state = GAME_STATE_PLAYING;

    char prompt_text[128];
    prompt_text[0] = '\0';
    str_concat(prompt_text, sizeof(prompt_text), "Draw a: ");
    str_concat(prompt_text, sizeof(prompt_text), g_game.drawing_prompts[g_game.current_prompt_index]);
    label_set_text(g_game.label_prompt, prompt_text);
    label_auto_size(g_game.label_prompt);
    container_update_layout(g_game.root_container);

    widget_set_visible(g_game.label_prompt, true);
    widget_set_visible(g_game.label_result, false);

    widget_set_visible(g_game.button_guess, true);
    widget_set_visible(g_game.button_clear, true);
    widget_set_visible(g_game.button_retry, false);

    canvas_clear(g_game.canvas);

    g_game.needs_redraw = true;
}

void game_set_prompt(int index)
{
    g_game.current_prompt_index = index;
}

void game_send_guess(int guess_index)
{
    if (!g_game.initialized || g_game.state != GAME_STATE_WAITING_FOR_GUESS)
        return;

    const char *guess = g_game.drawing_prompts[guess_index];
    bool correct = (guess_index == (int)g_game.current_prompt_index);

    if (correct)
    {
        g_game.score++;
    }

    char result_text[256];
    result_text[0] = '\0';
    if (correct)
    {
        str_concat(result_text, sizeof(result_text), "Correct! I guessed: ");
        str_concat(result_text, sizeof(result_text), guess);
    }
    else
    {
        str_concat(result_text, sizeof(result_text), "Wrong! I guessed: ");
        str_concat(result_text, sizeof(result_text), guess);
        str_concat(result_text, sizeof(result_text), " (was: ");
        str_concat(result_text, sizeof(result_text), g_game.drawing_prompts[g_game.current_prompt_index]);
        str_concat(result_text, sizeof(result_text), ")");
    }

    label_set_text(g_game.label_result, result_text);
    label_auto_size(g_game.label_result);

    widget_set_visible(g_game.label_result, true);
    widget_set_visible(g_game.label_prompt, true);

    g_game.state = GAME_STATE_RESULT;
    widget_set_visible(g_game.button_guess, false);
    widget_set_visible(g_game.button_clear, false);
    widget_set_visible(g_game.button_retry, true);

    g_game.needs_redraw = true;
}

bool game_render(Framebuffer *framebuffer)
{
    if (!g_game.initialized || !framebuffer)
        return false;

    static bool first = true;
    if (g_game.needs_redraw)
    {
        if (first) framebuffer_clear(framebuffer, COLOR_RGB(50, 50, 50));
        first = false;

        widget_render(g_game.root_container, framebuffer);

        g_game.needs_redraw = false;

        for (int i = 0; i < framebuffer->dirty_rect_count; i++) {
            DirtyRect rect = framebuffer->dirty_rects[i];
            for (int y = rect.y; y < rect.y + rect.height; y++) {
                for (int x = rect.x; x < rect.x + rect.width; x++) {
                    framebuffer->pixels[y * framebuffer->width + x] = COLOR_RGB(50, 50, 50);
                }
            }
        }
        framebuffer->dirty_rect_count = 0;

        return true;
    }

    return false;
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

    if (g_game.state == GAME_STATE_PLAYING && widget_contains_point(g_game.canvas, x, y))
    {
        g_game.is_drawing = true;
        canvas_draw_at(g_game.canvas, x, y);
        g_game.needs_redraw = true;
        return true;
    }

    widget_handle_click(g_game.button_guess, x, y);
    widget_handle_click(g_game.button_clear, x, y);
    widget_handle_click(g_game.button_retry, x, y);

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
            float src_x = out_x * x_scale;
            float src_y = out_y * y_scale;

            int x0 = (int)src_x;
            int y0 = (int)src_y;
            int x1 = x0 + 1;
            int y1 = y0 + 1;

            if (x1 >= canvas_width)
                x1 = canvas_width - 1;
            if (y1 >= canvas_height)
                y1 = canvas_height - 1;

            float fx = src_x - x0;
            float fy = src_y - y0;

            // Extract grayscale value from 32-bit colors (using red channel)
            uint8_t p00 = COLOR_GET_R(canvas_data->pixels[y0 * canvas_width + x0]);
            uint8_t p10 = COLOR_GET_R(canvas_data->pixels[y0 * canvas_width + x1]);
            uint8_t p01 = COLOR_GET_R(canvas_data->pixels[y1 * canvas_width + x0]);
            uint8_t p11 = COLOR_GET_R(canvas_data->pixels[y1 * canvas_width + x1]);

            float p0 = p00 * (1.0f - fx) + p10 * fx;
            float p1 = p01 * (1.0f - fx) + p11 * fx;
            float result = p0 * (1.0f - fy) + p1 * fy;

            output_buffer[out_y * 28 + out_x] = (uint8_t)result;
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
        g_game.state = GAME_STATE_WAITING_FOR_GUESS;

        uint8_t canvas_28x28[28 * 28];
        game_get_canvas_28x28(canvas_28x28);
        g_game.guess_callback(canvas_28x28, g_game.callback_user_data);
    }
    else
    {
        g_game.state = GAME_STATE_WAITING_FOR_GUESS;
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
