#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "color.h"
#include "framebuffer.h"
#include "game.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 272
#define WINDOW_HEIGHT 480
#define FRAMEBUFFER_SIZE (WINDOW_WIDTH * WINDOW_HEIGHT)

static Framebuffer framebuffer;
static Uint64 last_guess_time = 0;

static const char *DRAWING_PROMPTS[] = {"Cat",    "House", "Tree", "Car",   "Sun",
                                        "Flower", "Star",  "Fish", "Heart", "Circle"};
#define NUM_PROMPTS 10

void on_guess_request(const uint8_t *canvas_28x28, void *user_data)
{
    game_send_guess(rand() % NUM_PROMPTS);
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

    Color *pixels = malloc(sizeof(Color) * FRAMEBUFFER_SIZE);
    if (!pixels)
    {
        SDL_Log("Failed to allocate framebuffer");
        return SDL_APP_FAILURE;
    }
    framebuffer = (Framebuffer){pixels, WINDOW_WIDTH, WINDOW_HEIGHT};

    GameConfig config = {
        .drawing_prompts = DRAWING_PROMPTS,
        .num_prompts = NUM_PROMPTS,
        .window_width = WINDOW_WIDTH,
        .window_height = WINDOW_HEIGHT,
        .canvas_width = 0,
        .canvas_height = 0,
        .guess_callback = on_guess_request,
        .callback_user_data = NULL,
    };

    if (!game_init(&config))
    {
        SDL_Log("Failed to initialize game");
        free(pixels);
        return SDL_APP_FAILURE;
    }

    last_guess_time = SDL_GetTicks();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_UP) {
        game_on_retry_click(NULL, NULL);
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        int mouse_x = (int)event->button.x;
        int mouse_y = (int)event->button.y;
        game_handle_mouse_down(mouse_x, mouse_y);
    }

    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP && event->button.button == SDL_BUTTON_LEFT)
    {
        int mouse_x = (int)event->button.x;
        int mouse_y = (int)event->button.y;
        game_handle_mouse_up(mouse_x, mouse_y);
    }

    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        int mouse_x = (int)event->motion.x;
        int mouse_y = (int)event->motion.y;
        game_handle_mouse_move(mouse_x, mouse_y);
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 current_time = SDL_GetTicks();
    if (current_time - last_guess_time >= 1000)
    {
        on_guess_request(NULL, NULL);
        last_guess_time = current_time;
    }

    game_render(&framebuffer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (int y = 0; y < WINDOW_HEIGHT; y++)
    {
        for (int x = 0; x < WINDOW_WIDTH; x++)
        {
            Color pixel = framebuffer.pixels[y * WINDOW_WIDTH + x];
            uint8_t r = COLOR_GET_R(pixel);
            uint8_t g = COLOR_GET_G(pixel);
            uint8_t b = COLOR_GET_B(pixel);
            uint8_t a = COLOR_GET_A(pixel);
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderPoint(renderer, x, y);
        }
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    game_cleanup();

    if (framebuffer.pixels)
    {
        free(framebuffer.pixels);
    }
}
