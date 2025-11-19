#include "menu.h"
#include "color.h"
#include "font_medium.h"
#include "widgets/button.h"
#include "widgets/container.h"
#include "widgets/hbox.h"
#include "widgets/image_widget.h"
#include "widgets/vbox.h"
#include "widgets/widget.h"
#include <stdlib.h>

// Include character images for title
#include "s_char.h"
#include "k_char.h"
#include "i_char.h"
#include "d_char.h"
#include "a_char.h"
#include "l_char.h"
#include "e_char.h"

#define SPACING_LG 8
#define SPACING_XL 16

static struct
{
    Widget *menu_container;
    Widget *label_title;
    Widget *button_play;
} g_menu = {0};

Widget *menu_init(const GameConfig *config)
{
    if (!config)
    {
        return NULL;
    }

    const bdf_font_t *button_font = config->button_font ? config->button_font : &font_medium_font;

    // Create menu container
    g_menu.menu_container = vbox_create(0, 0, config->window_width, config->window_height);
    if (!g_menu.menu_container)
    {
        menu_cleanup();
        return NULL;
    }
    container_set_padding(g_menu.menu_container, SPACING_LG);
    container_set_spacing(g_menu.menu_container, SPACING_XL);
    container_set_alignment(g_menu.menu_container, ALIGN_CENTER);
    container_set_justify(g_menu.menu_container, ALIGN_CENTER);

    // Create title as hbox with character images
    g_menu.label_title = hbox_create(0, 0, config->window_width, 32);
    if (!g_menu.label_title)
    {
        menu_cleanup();
        return NULL;
    }
    container_set_spacing(g_menu.label_title, 2);
    container_set_justify(g_menu.label_title, ALIGN_CENTER);

    // Set up floating animation for the title
    container_set_animation(g_menu.label_title, ANIMATION_FLOATING);
    container_set_animation_speed(g_menu.label_title, 60); // 60 degrees per second

    // Add character images for "SKIDADDLE"
    const Image *title_chars[] = {&s_image, &k_image, &i_image, &d_image, &a_image, &d_image, &d_image, &l_image, &e_image};
    for (int i = 0; i < 9; i++)
    {
        Widget *char_widget = image_widget_create(0, 0, title_chars[i]);
        if (!char_widget)
        {
            menu_cleanup();
            return NULL;
        }
        container_add_child(g_menu.label_title, char_widget);
    }

    container_add_child(g_menu.menu_container, g_menu.label_title);

    g_menu.button_play = button_create_auto(0, 0, "Play", button_font);
    if (!g_menu.button_play)
    {
        menu_cleanup();
        return NULL;
    }
    button_set_background_color(g_menu.button_play, COLOR_RGB(46, 170, 80));
    button_set_text_color(g_menu.button_play, COLOR_WHITE);
    button_set_border(g_menu.button_play, COLOR_RGB(35, 118, 54), 1);
    button_set_on_click(g_menu.button_play, game_on_play_click, NULL);
    container_add_child(g_menu.menu_container, g_menu.button_play);

    return g_menu.menu_container;
}

void menu_cleanup(void)
{
    // Widgets are cleaned up by parent container, just clear references
    g_menu.menu_container = NULL;
    g_menu.label_title = NULL;
    g_menu.button_play = NULL;
}

void menu_update(float delta_time)
{
    if (g_menu.label_title)
    {
        container_update_animation(g_menu.label_title, delta_time);
    }
}
