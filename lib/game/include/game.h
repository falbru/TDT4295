#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "font_types.h"
#include "framebuffer.h"
#include "widgets/widget.h"
#include <stdbool.h>
#include <stdint.h>

typedef void (*game_guess_callback_t)(const uint8_t *canvas_28x28, void *user_data);

typedef enum
{
    GAME_STATE_PLAYING,
    GAME_STATE_WAITING_FOR_GUESS,
    GAME_STATE_RESULT,
    GAME_STATE_MENU
} GameState;

typedef struct
{
    const char **drawing_prompts;
    unsigned int num_prompts;
    unsigned int window_width;
    unsigned int window_height;
    unsigned int canvas_width;
    unsigned int canvas_height;
    game_guess_callback_t guess_callback;
    void *callback_user_data;
    const bdf_font_t *label_font;
    const bdf_font_t *button_font;
} GameConfig;

bool game_init(const GameConfig *config);

void game_cleanup(void);

void game_start_new_round(void);

void game_update(float delta_time);

bool game_render(Framebuffer *framebuffer);

bool game_handle_mouse_down(unsigned int x, unsigned int y);
bool game_handle_mouse_up(unsigned int x, unsigned int y);
bool game_handle_mouse_move(unsigned int x, unsigned int y);

void game_get_canvas_28x28(uint8_t *output_buffer);

void game_send_guess(int guess_index);

int game_get_state(void);

bool game_is_initialized(void);

void game_set_random_seed(unsigned int seed);

void game_on_guess(Widget *widget, void *user_data);
void game_on_retry(Widget *widget, void *user_data);
void game_on_play(Widget *widget, void *user_data);
void game_on_menu(Widget *widget, void *user_data);
void game_on_skip(Widget *widget, void *user_data);

#endif
