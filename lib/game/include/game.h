#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "font_types.h"
#include "framebuffer.h"
#include <stdbool.h>
#include <stdint.h>

typedef int (*game_guess_callback_t)(const uint8_t *canvas_28x28, void *user_data);

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

bool game_render(Framebuffer *framebuffer);

bool game_needs_redraw(void);

void game_request_redraw(void);

bool game_handle_mouse_down(unsigned int x, unsigned int y);
bool game_handle_mouse_up(unsigned int x, unsigned int y);
bool game_handle_mouse_move(unsigned int x, unsigned int y);

void game_get_canvas_28x28(uint8_t *output_buffer);

int game_get_current_prompt_index(void);

int game_get_state(void);

#endif
