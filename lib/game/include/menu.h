#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "game.h"
#include "widgets/widget.h"

Widget *menu_init(const GameConfig *config);
void menu_cleanup(void);
void menu_update(float delta_time);

#endif
