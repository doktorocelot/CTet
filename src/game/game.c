#include <SDL.h>
#include "game.h"
#include "stdbool.h"
#include "engine/engine.h"
#include "engine/rendering.h"

//temp until custom controls
#ifdef OCELOT_CONTROLS
#define CONTROL_HDROP SDLK_UP
#else
#define CONTROL_HDROP SDLK_SPACE
#endif

struct Game {
    SDL_Window *window;
    SDL_Renderer *sdlRenderer;
    Engine *engine;
    Rendering rendering;
    bool isPaused;
};



static void game_setupRenderer(Game *game);

static void game_makeFullscreen(Game *game);

Game *game_create() {
    Game *game = malloc(sizeof(Game));

    SDL_Init(SDL_INIT_VIDEO);

    game->window = SDL_CreateWindow(
            "CTet",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            GAME_ENGINE_WIDTH, GAME_ENGINE_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    game->sdlRenderer = SDL_CreateRenderer(
            game->window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    game->engine = NULL;

    game->rendering = (Rendering) {0};

    game->isPaused = false;

    game_setupRenderer(game);

    return game;
}

void game_destroy(Game *game) {
    SDL_DestroyRenderer(game->sdlRenderer);
    SDL_DestroyWindow(game->window);
    engine_destroy(game->engine);
    free(game);
    SDL_Quit();
}

void game_run(Game *game) {
    SDL_Event event;

    bool isRunning = true;

    Uint64 previousTime = SDL_GetTicks64();

    SDL_Renderer *renderer = game->sdlRenderer;

    game->engine = engine_create();

    float redAccumulator = 0;
    bool isRed = false;

    while (isRunning) {
        bool isPaused = game->isPaused;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            } else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                switch (event.key.keysym.sym) {
                    case SDLK_q:
                        isRunning = false;
                        break;
                    case SDLK_F11:
                    case SDLK_f:
                        game_makeFullscreen(game);
                        break;
                    case SDLK_r:
                        if (game->isPaused) game->isPaused = false;
                        engine_reset(game->engine);
                        break;
                    case SDLK_ESCAPE:
                        if (game->engine->isDead) break;
                        game->isPaused ^= true;
                        break;

                        //game
                    case SDLK_RIGHT:
                        if (isPaused) break;
                        engine_onShiftRightDown(game->engine);
                        break;
                    case SDLK_LEFT:
                        if (isPaused) break;
                        engine_onShiftLeftDown(game->engine);
                        break;
                    case CONTROL_HDROP:
                        if (isPaused) break;
                        engine_onHardDrop(game->engine);
                        break;
                    case SDLK_z:
                        if (isPaused) break;
                        engine_onRotateLeft(game->engine);
                        break;

                    case SDLK_x:
#ifndef OCELOT_CONTROLS
                        case SDLK_UP:
#endif
                        if (isPaused) break;
                        engine_onRotateRight(game->engine);
                        break;
                    case SDLK_DOWN:
                        if (isPaused) break;
                        engine_onSoftDropDown(game->engine);
                        break;
                    case SDLK_c:
                    case SDLK_LSHIFT:
                        if (isPaused) break;
                        engine_onHoldDown(game->engine);
                        break;
                }
            } else if (event.type == SDL_KEYUP && event.key.repeat == 0) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        engine_onShiftRightUp(game->engine);
                        break;
                    case SDLK_LEFT:
                        engine_onShiftLeftUp(game->engine);
                        break;
                    case SDLK_DOWN:
                        engine_onSoftDropUp(game->engine);
                        break;
                }
            }
        }


        // CALCULATE DELTA
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (float) currentTime - (float) previousTime;
        previousTime = currentTime;

        // RUN ENGINE
        if (game->engine != NULL && !isPaused) {
            engine_tick(game->engine, deltaTime);
            if (game->engine->isDead) game->isPaused = true;
        }

        // DRAW GAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        rendering_tick(&game->rendering, deltaTime);

        if (game->engine != NULL) {
            rendering_drawEngineInSdl(&game->rendering, renderer, game->engine, game->isPaused);
        }

        // SWAP BUFFERS
        SDL_RenderPresent(renderer);
    }
}


void game_makeFullscreen(Game *game) {
    Uint32 fullscreenFlag = SDL_GetWindowFlags(game->window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_SetWindowFullscreen(game->window, fullscreenFlag ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void game_setupRenderer(Game *game) {
    SDL_Renderer *renderer = game->sdlRenderer;

    SDL_RenderSetLogicalSize(renderer, GAME_ENGINE_WIDTH, GAME_ENGINE_HEIGHT);
}


