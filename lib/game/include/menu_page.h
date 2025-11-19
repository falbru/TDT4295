#ifndef MENU_PAGE_H_INCLUDED
#define MENU_PAGE_H_INCLUDED

#include "game.h"
#include "widgets/widget.h"

Widget *menu_page_init(const GameConfig *config);
void menu_page_cleanup(void);
void menu_page_update(float delta_time);

#endif
