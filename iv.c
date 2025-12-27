#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_main.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static unsigned char *data = NULL;
int width, height, channels;

SDL_AppResult SDL_AppIterate(void *appstate)
{

    /* you have to draw the whole window every frame. Clearing it makes sure the whole thing is sane. */
    SDL_RenderClear(renderer);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width * 4; j += 4) {
            int offset = i * width * 4 + j;
            float r = data[offset];
            float g = data[offset + 1];
            float b = data[offset + 2];
            float a = data[offset + 3];
            SDL_SetRenderDrawColor(renderer, r, g, b, a);
            SDL_RenderPoint(renderer, (float)j / 4, i);
        }
    }

    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch (event->type) {
        case SDL_EVENT_QUIT:  /* triggers on last window close and other things. End the program. */
            return SDL_APP_SUCCESS;

        case SDL_EVENT_KEY_DOWN:  /* quit if user hits ESC key */
            if (event->key.scancode == SDL_SCANCODE_ESCAPE) {
                return SDL_APP_SUCCESS;
            }
            break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

    if (argc < 2) {
        SDL_Log("Not enough args provided.\n");
        return SDL_APP_FAILURE;
    }
    const char* filename = argv[1];
    data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (!data) { 
        SDL_Log("Couldn't load file data\n"); 
        return SDL_APP_FAILURE;
    }

    SDL_SetAppMetadata("Image Viewer", "1.0", "com.dot.iv");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(filename, width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    stbi_image_free(data);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
