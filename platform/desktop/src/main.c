#include <SDL3/SDL_rect.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 272

#include "framebuffer.h"
#include "primitives/image.h"
#include "primitives/text.h"
#include "primitives/rectangle.h"
#include "widgets/button.h"
#include "widgets/label.h"
#include "color.h"
#include "font.h"

Framebuffer framebuffer;

#define FRAMEBUFFER_SIZE WINDOW_WIDTH *WINDOW_HEIGHT

static Widget *button1 = NULL;
static Widget *button2 = NULL;
static Widget *label_title = NULL;
static Widget *label_counter = NULL;

static int click_count = 0;
static bool needs_redraw = true;

void on_button1_click(Widget *widget, void *user_data)
{
    click_count++;
    printf("Button 1 clicked! Total clicks: %d\n", click_count);
    needs_redraw = true;
}

void on_button2_click(Widget *widget, void *user_data)
{
    printf("Button 2 clicked! Resetting counter.\n");
    click_count = 0;
    needs_redraw = true;
}

void render_framebuffer()
{
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++)
    {
        framebuffer.pixels[i] = 0;
    }

    char counter_text[64];
    snprintf(counter_text, sizeof(counter_text), "Clicks: %d", click_count);
    label_set_text(label_counter, counter_text);
    label_auto_size(label_counter);

    widget_render(label_title, &framebuffer);
    widget_render(label_counter, &framebuffer);
    widget_render(button1, &framebuffer);
    widget_render(button2, &framebuffer);

    renderImage(10, 180, &framebuffer);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("GUI Library Example", "1.0", "com.example.renderer-points");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("GUI Demo", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    uint8_t *pixels = malloc(sizeof(uint8_t) * FRAMEBUFFER_SIZE);
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++)
    {
        pixels[i] = 0;
    }
    framebuffer = (Framebuffer){pixels, WINDOW_WIDTH, WINDOW_HEIGHT};

    label_title = label_create_auto(10, 10, "Widget Demo - Click the buttons!", &font_font);
    label_set_color(label_title, COLOR_WHITE);

    label_counter = label_create_auto(10, 30, "Clicks: 0", &font_font);
    label_set_color(label_counter, COLOR_WHITE);

    button1 = button_create_auto(10, 60, "Click Me!", &font_font);
    button_set_background_color(button1, COLOR_GRAY_75);
    button_set_text_color(button1, COLOR_BLACK);
    button_set_border(button1, COLOR_WHITE, 2);
    button_set_on_click(button1, on_button1_click, NULL);

    int button2_x = button1->x + button1->width + 10;
    button2 = button_create_auto(button2_x, 60, "Reset Count", &font_font);
    button_set_background_color(button2, COLOR_GRAY_75);
    button_set_text_color(button2, COLOR_BLACK);
    button_set_border(button2, COLOR_WHITE, 2);
    button_set_on_click(button2, on_button2_click, NULL);

    render_framebuffer();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        int mouse_x = (int)event->button.x;
        int mouse_y = (int)event->button.y;

        printf("Mouse clicked at (%d, %d)\n", mouse_x, mouse_y);

        widget_handle_click(button1, mouse_x, mouse_y);
        widget_handle_click(button2, mouse_x, mouse_y);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    if (needs_redraw)
    {
        render_framebuffer();
        needs_redraw = false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            uint8_t pixel = framebuffer.pixels[y * WINDOW_WIDTH + x];
            SDL_SetRenderDrawColor(renderer, pixel, pixel, pixel, SDL_ALPHA_OPAQUE);
            SDL_RenderPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (label_title)
    {
        widget_destroy(label_title);
        free(label_title);
    }
    if (label_counter)
    {
        widget_destroy(label_counter);
        free(label_counter);
    }
    if (button1)
    {
        widget_destroy(button1);
        free(button1);
    }
    if (button2)
    {
        widget_destroy(button2);
        free(button2);
    }

    if (framebuffer.pixels)
    {
        free(framebuffer.pixels);
    }

    printf("Application closed. Total clicks: %d\n", click_count);
}
