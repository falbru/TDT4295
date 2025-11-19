#include "game_page.h"
#include "color.h"
#include "font_medium.h"
#include "widgets/button.h"
#include "widgets/canvas.h"
#include "widgets/container.h"
#include "widgets/hbox.h"
#include "widgets/label.h"
#include "widgets/vbox.h"
#include "widgets/widget.h"
#include <stdlib.h>
#include <string.h>
#include "font_small.h"

#define DEFAULT_CANVAS_WIDTH 28 * 8
#define DEFAULT_CANVAS_HEIGHT 28 * 8
#define SPACING_MD 4
#define SPACING_LG 8
#define SPACING_XL 16

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
    unsigned int current_prompt_index;
    unsigned int score;

    Widget *game_container;
    Widget *button_container;
    Widget *action_button_container;
    Widget *label_round;
    Widget *label_prompt;
    Widget *label_result;
    Widget *button_retry;
    Widget *button_menu;
    Widget *button_skip;
    Widget *canvas;

    const bdf_font_t *label_font;
} g_game_page = {0};

Widget *game_page_init(const GameConfig *config)
{
    if (!config || !config->drawing_prompts || config->num_prompts == 0)
    {
        return NULL;
    }

    g_game_page.drawing_prompts = config->drawing_prompts;
    g_game_page.num_prompts = config->num_prompts;
    g_game_page.current_prompt_index = 0;
    g_game_page.score = 0;

    const bdf_font_t *label_font = config->label_font ? config->label_font : &font_medium_font;
    const bdf_font_t *button_font = config->button_font ? config->button_font : &font_medium_font;
    g_game_page.label_font = label_font;

    unsigned int canvas_width = config->canvas_width > 0 ? config->canvas_width : DEFAULT_CANVAS_WIDTH;
    unsigned int canvas_height = config->canvas_height > 0 ? config->canvas_height : DEFAULT_CANVAS_HEIGHT;

    // Create game container
    g_game_page.game_container = vbox_create(0, 0, config->window_width, config->window_height);
    if (!g_game_page.game_container)
    {
        game_page_cleanup();
        return NULL;
    }
    container_set_padding(g_game_page.game_container, SPACING_LG);
    container_set_spacing(g_game_page.game_container, SPACING_XL);
    container_set_alignment(g_game_page.game_container, ALIGN_CENTER);
    widget_set_visible(g_game_page.game_container, false);

    g_game_page.label_round = label_create_auto(0, 0, "Round 0", &font_small_font);
    if (!g_game_page.label_round)
    {
        game_page_cleanup();
        return NULL;
    }
    label_set_color(g_game_page.label_round, COLOR_GRAY_50);
    container_add_child(g_game_page.game_container, g_game_page.label_round);

    g_game_page.label_prompt = label_create_auto(0, 0, "Draw: ", label_font);
    if (!g_game_page.label_prompt)
    {
        game_page_cleanup();
        return NULL;
    }
    label_set_color(g_game_page.label_prompt, COLOR_BLACK);
    container_add_child(g_game_page.game_container, g_game_page.label_prompt);

    g_game_page.canvas = canvas_create(0, 0, canvas_width, canvas_height);
    if (!g_game_page.canvas)
    {
        game_page_cleanup();
        return NULL;
    }
    canvas_set_brush_size(g_game_page.canvas, 7);
    canvas_set_brush_color(g_game_page.canvas, COLOR_BLACK);
    canvas_set_background_color(g_game_page.canvas, COLOR_BLACK);
    canvas_set_border(g_game_page.canvas, COLOR_BLACK, 1);
    container_add_child(g_game_page.game_container, g_game_page.canvas);

    g_game_page.button_container = hbox_create(0, 0, config->window_width, 40);
    if (!g_game_page.button_container)
    {
        game_page_cleanup();
        return NULL;
    }
    container_set_spacing(g_game_page.button_container, SPACING_MD);
    container_set_justify(g_game_page.button_container, ALIGN_CENTER);

    g_game_page.label_result = label_create_auto(0, 0, "", label_font);
    if (!g_game_page.label_result)
    {
        game_page_cleanup();
        return NULL;
    }
    label_set_color(g_game_page.label_result, COLOR_BLACK);
    container_add_child(g_game_page.game_container, g_game_page.label_result);

    // Create action button container for menu and skip buttons
    g_game_page.action_button_container = hbox_create(0, 0, config->window_width, 40);
    if (!g_game_page.action_button_container)
    {
        game_page_cleanup();
        return NULL;
    }
    container_set_spacing(g_game_page.action_button_container, SPACING_MD);
    container_set_justify(g_game_page.action_button_container, ALIGN_CENTER);

    g_game_page.button_menu = button_create_auto(0, 0, "Menu", &font_small_font);
    if (!g_game_page.button_menu)
    {
        game_page_cleanup();
        return NULL;
    }
    button_set_background_color(g_game_page.button_menu, COLOR_YELLOW);
    button_set_text_color(g_game_page.button_menu, COLOR_BLACK);
    button_set_border(g_game_page.button_menu, COLOR_GRAY_25, 2);
    button_set_on_click(g_game_page.button_menu, game_on_menu_click, NULL);
    widget_set_visible(g_game_page.button_menu, true);
    container_add_child(g_game_page.action_button_container, g_game_page.button_menu);

    g_game_page.button_skip = button_create_auto(0, 0, "Skip", &font_small_font);
    if (!g_game_page.button_skip)
    {
        game_page_cleanup();
        return NULL;
    }
    button_set_background_color(g_game_page.button_skip, COLOR_YELLOW);
    button_set_text_color(g_game_page.button_skip, COLOR_BLACK);
    button_set_border(g_game_page.button_skip, COLOR_GRAY_25, 2);
    button_set_on_click(g_game_page.button_skip, game_on_skip_click, NULL);
    widget_set_visible(g_game_page.button_skip, true);
    container_add_child(g_game_page.action_button_container, g_game_page.button_skip);

    g_game_page.button_retry = button_create_auto(0, 0, "Play again", &font_small_font);
    if (!g_game_page.button_retry)
    {
        game_page_cleanup();
        return NULL;
    }
    button_set_background_color(g_game_page.button_retry, COLOR_RGB(46, 170, 80));
    button_set_text_color(g_game_page.button_retry, COLOR_WHITE);
    button_set_border(g_game_page.button_retry, COLOR_RGB(35, 118, 54), 2);
    button_set_on_click(g_game_page.button_retry, game_on_retry_click, NULL);
    widget_set_visible(g_game_page.button_retry, false);
    container_add_child(g_game_page.action_button_container, g_game_page.button_retry);

    container_add_child(g_game_page.game_container, g_game_page.action_button_container);
    container_add_child(g_game_page.game_container, g_game_page.button_container);

    return g_game_page.game_container;
}

static void uint_to_str(unsigned int num, char *str, size_t size)
{
    if (size == 0) return;

    char temp[16];
    int i = 0;

    if (num == 0)
    {
        temp[i++] = '0';
    }
    else
    {
        while (num > 0 && i < 15)
        {
            temp[i++] = '0' + (num % 10);
            num /= 10;
        }
    }

    int j = 0;
    while (i > 0 && j < (int)size - 1)
    {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
}

static void update_round_label(void)
{
    char round_text[32];
    round_text[0] = '\0';
    str_concat(round_text, sizeof(round_text), "Round ");

    char score_str[16];
    uint_to_str(g_game_page.score, score_str, sizeof(score_str));
    str_concat(round_text, sizeof(round_text), score_str);

    label_set_text(g_game_page.label_round, round_text);
    label_auto_size(g_game_page.label_round);
    container_update_layout(g_game_page.game_container);
}

void game_page_cleanup(void)
{
    // Widgets are cleaned up by parent container, just clear references
    g_game_page.game_container = NULL;
    g_game_page.button_container = NULL;
    g_game_page.action_button_container = NULL;
    g_game_page.label_round = NULL;
    g_game_page.label_prompt = NULL;
    g_game_page.label_result = NULL;
    g_game_page.button_retry = NULL;
    g_game_page.button_menu = NULL;
    g_game_page.button_skip = NULL;
    g_game_page.canvas = NULL;
}

void game_page_reset_score(void)
{
    g_game_page.score = 0;
    update_round_label();
}

void game_page_start_new_round(void)
{
    g_game_page.current_prompt_index = rand() % g_game_page.num_prompts;

    char prompt_text[128];
    prompt_text[0] = '\0';
    str_concat(prompt_text, sizeof(prompt_text), "Draw a: ");
    str_concat(prompt_text, sizeof(prompt_text), g_game_page.drawing_prompts[g_game_page.current_prompt_index]);
    label_set_text(g_game_page.label_prompt, prompt_text);
    label_auto_size(g_game_page.label_prompt);
    container_update_layout(g_game_page.game_container);

    widget_set_visible(g_game_page.label_prompt, true);
    widget_set_visible(g_game_page.label_result, false);
    widget_set_visible(g_game_page.button_retry, false);
    widget_set_visible(g_game_page.button_menu, true);
    widget_set_visible(g_game_page.button_skip, true);

    canvas_clear(g_game_page.canvas);
}

int game_page_send_guess(int guess_index)
{
    const char *guess = g_game_page.drawing_prompts[guess_index];
    bool correct = (guess_index == (int)g_game_page.current_prompt_index);

    char result_text[256];
    result_text[0] = '\0';
    str_concat(result_text, sizeof(result_text), guess);

    label_set_text(g_game_page.label_result, result_text);
    label_auto_size(g_game_page.label_result);

    widget_set_visible(g_game_page.label_result, true);
    widget_set_visible(g_game_page.label_prompt, true);
    widget_set_visible(g_game_page.button_menu, true);

    if (correct)
    {
        g_game_page.score++;
        update_round_label();
        widget_set_visible(g_game_page.button_retry, true);
        widget_set_visible(g_game_page.button_skip, false);
        return true;
    }
    else
    {
        widget_set_visible(g_game_page.button_skip, true);
        widget_set_visible(g_game_page.button_retry, false);
    }
    return false;
}

void game_page_get_canvas_28x28(uint8_t *output_buffer)
{
    if (!g_game_page.canvas || !output_buffer)
        return;

    CanvasData *canvas_data = (CanvasData *)g_game_page.canvas->data;
    if (!canvas_data || !canvas_data->pixels)
        return;

    int canvas_width = g_game_page.canvas->width;
    int canvas_height = g_game_page.canvas->height;

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

Widget *game_page_get_canvas(void)
{
    return g_game_page.canvas;
}
