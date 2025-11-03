#include "widgets/container.h"
#include "widgets/hbox.h"
#include "widgets/vbox.h"
#include <SDL3/SDL_rect.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 272

#include "color.h"
#include "font.h"
#include "framebuffer.h"
#include "widgets/button.h"
#include "widgets/canvas.h"
#include "widgets/label.h"

Framebuffer framebuffer;

#define FRAMEBUFFER_SIZE WINDOW_WIDTH *WINDOW_HEIGHT

static const char *DRAWING_PROMPTS[] = {"Cat",    "House", "Tree", "Car",   "Sun",
                                        "Flower", "Star",  "Fish", "Heart", "Circle"};
#define NUM_PROMPTS 10

typedef enum
{
    GAME_STATE_PLAYING,
    GAME_STATE_RESULT
} GameState;

static Widget *container = NULL;
static Widget *button_container = NULL;
static Widget *label_prompt = NULL;
static Widget *label_result = NULL;
static Widget *button_guess = NULL;
static Widget *button_clear = NULL;
static Widget *button_retry = NULL;
static Widget *canvas = NULL;

static GameState game_state = GAME_STATE_PLAYING;
static int current_target_index = 0;
static bool needs_redraw = true;
static bool is_drawing = false;

void start_new_round()
{
    current_target_index = rand() % NUM_PROMPTS;
    game_state = GAME_STATE_PLAYING;

    char prompt_text[128];
    snprintf(prompt_text, sizeof(prompt_text), "Draw a: %s", DRAWING_PROMPTS[current_target_index]);
    label_set_text(label_prompt, prompt_text);
    label_auto_size(label_prompt);

    label_set_text(label_result, "");

    widget_set_visible(button_guess, true);
    widget_set_visible(button_clear, true);
    widget_set_visible(button_retry, false);

    canvas_clear(canvas);
    needs_redraw = true;
}

void on_guess_click(Widget *widget, void *user_data)
{
    int guess_index = rand() % NUM_PROMPTS;
    const char *guess = DRAWING_PROMPTS[guess_index];

    bool correct = (guess_index == current_target_index);

    char result_text[256];
    if (correct)
    {
        snprintf(result_text, sizeof(result_text), "Correct! I guessed: %s", guess);
    }
    else
    {
        snprintf(result_text, sizeof(result_text), "Incorrect! I guessed: %s", guess,
                 DRAWING_PROMPTS[current_target_index]);
    }

    label_set_text(label_result, result_text);
    label_auto_size(label_result);

    game_state = GAME_STATE_RESULT;
    widget_set_visible(button_guess, false);
    widget_set_visible(button_clear, false);
    widget_set_visible(button_retry, true);

    needs_redraw = true;
}

void on_clear_canvas_click(Widget *widget, void *user_data)
{
    canvas_clear(canvas);
    needs_redraw = true;
}

void on_retry_click(Widget *widget, void *user_data)
{
    start_new_round();
}

void render_framebuffer()
{
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++)
    {
        framebuffer.pixels[i] = 50;
    }

    // widget_render(label_prompt, &framebuffer);
    // widget_render(canvas, &framebuffer);
    widget_render(container, &framebuffer);
    // widget_render(button_guess, &framebuffer);
    // widget_render(button_clear, &framebuffer);
    // widget_render(button_retry, &framebuffer);
    // widget_render(label_result, &framebuffer);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    srand(time(NULL));

    SDL_SetAppMetadata("Drawing Game", "1.0", "com.example.drawing-game");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Drawing Game", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
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

    label_prompt = label_create_auto(10, 10, "Draw a: Cat", &font_font);
    label_set_color(label_prompt, COLOR_WHITE);

    container = vbox_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    container_add_child(container, label_prompt);
    container_set_padding(container, 8);
    container_set_spacing(container, 8);

    canvas = canvas_create(10, 35, 460, 150);
    canvas_set_brush_size(canvas, 3);
    canvas_set_brush_color(canvas, COLOR_BLACK);
    canvas_set_border(canvas, COLOR_WHITE, 2);
    container_add_child(container, canvas);

    button_container = hbox_create(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    container_set_spacing(button_container, 4);

    button_guess = button_create_auto(0, 0, "Guess", &font_font);
    button_set_background_color(button_guess, COLOR_GRAY_75);
    button_set_text_color(button_guess, COLOR_BLACK);
    button_set_border(button_guess, COLOR_WHITE, 2);
    button_set_on_click(button_guess, on_guess_click, NULL);
    container_add_child(button_container, button_guess);

    button_clear = button_create_auto(0, 0, "Clear Canvas", &font_font);
    button_set_background_color(button_clear, COLOR_GRAY_75);
    button_set_text_color(button_clear, COLOR_BLACK);
    button_set_border(button_clear, COLOR_WHITE, 2);
    button_set_on_click(button_clear, on_clear_canvas_click, NULL);
    container_add_child(button_container, button_clear);

    button_retry = button_create_auto(0, 0, "Try Again", &font_font);
    button_set_background_color(button_retry, COLOR_GRAY_75);
    button_set_text_color(button_retry, COLOR_BLACK);
    button_set_border(button_retry, COLOR_WHITE, 2);
    button_set_on_click(button_retry, on_retry_click, NULL);
    widget_set_visible(button_retry, false);
    container_add_child(button_container, button_retry);

    container_add_child(container, button_container);

    label_result = label_create_auto(10, 250, "", &font_font);
    label_set_color(label_result, COLOR_WHITE);
    container_add_child(container, label_result);

    start_new_round();

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

        if (event->button.button == SDL_BUTTON_LEFT)
        {
            if (widget_contains_point(canvas, mouse_x, mouse_y) && game_state == GAME_STATE_PLAYING)
            {
                is_drawing = true;
                canvas_draw_at(canvas, mouse_x, mouse_y);
                needs_redraw = true;
            }
            else
            {
                widget_handle_click(button_guess, mouse_x, mouse_y);
                widget_handle_click(button_clear, mouse_x, mouse_y);
                widget_handle_click(button_retry, mouse_x, mouse_y);
            }
        }
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
    {
        if (event->button.button == SDL_BUTTON_LEFT)
        {
            is_drawing = false;
        }
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        if (is_drawing && game_state == GAME_STATE_PLAYING)
        {
            int mouse_x = (int)event->motion.x;
            int mouse_y = (int)event->motion.y;

            canvas_draw_at(canvas, mouse_x, mouse_y);
            needs_redraw = true;
        }
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
    if (label_prompt)
    {
        widget_destroy(label_prompt);
        free(label_prompt);
    }
    if (label_result)
    {
        widget_destroy(label_result);
        free(label_result);
    }
    if (button_guess)
    {
        widget_destroy(button_guess);
        free(button_guess);
    }
    if (button_clear)
    {
        widget_destroy(button_clear);
        free(button_clear);
    }
    if (button_retry)
    {
        widget_destroy(button_retry);
        free(button_retry);
    }
    if (canvas)
    {
        widget_destroy(canvas);
        free(canvas);
    }

    if (framebuffer.pixels)
    {
        free(framebuffer.pixels);
    }
}
