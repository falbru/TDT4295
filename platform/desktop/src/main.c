#include <SDL3/SDL_rect.h>
#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 272

#include "framebuffer.h"
#include "image.h"
#include "text.h"
#include "font.h"

Framebuffer framebuffer;

#define FRAMEBUFFER_SIZE WINDOW_WIDTH *WINDOW_HEIGHT

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

    renderText("TDT4295 is fun!", 255, 0, 0, &font_font, &framebuffer);
    renderImage(0, 100, &framebuffer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
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
}
