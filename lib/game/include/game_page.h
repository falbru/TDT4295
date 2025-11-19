#ifndef GAME_PAGE_H_INCLUDED
#define GAME_PAGE_H_INCLUDED

#include "game.h"
#include "widgets/widget.h"
#include <stdint.h>

Widget *game_page_init(const GameConfig *config);
void game_page_cleanup(void);
void game_page_reset_round(void);
void game_page_start_new_round(void);
int game_page_send_guess(int guess_index);
void game_page_get_canvas_28x28(uint8_t *output_buffer);
Widget *game_page_get_canvas(void);

#endif
