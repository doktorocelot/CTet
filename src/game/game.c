#include <malloc.h>
#include "game.h"
#include "stdbool.h"
#include "engine/engine.h"

//temp until custom controls
#ifdef OCELOT_CONTROLS
#define CONTROL_HDROP SDLK_UP
#else
#define CONTROL_HDROP SDLK_SPACE
#endif

struct Game {
    Engine *engine;
    bool isPaused;
};



static void game_setupRenderer(Game *game);

static void game_makeFullscreen(Game *game);

Game *game_create() {
    Game *game = malloc(sizeof(Game));

    game->engine = NULL;


    game->isPaused = false;

    game_setupRenderer(game);

    return game;
}

void game_destroy(Game *game) {
    engine_destroy(game->engine);
    free(game);
}

void game_run(Game *game) {

    bool isRunning = true;

    game->engine = engine_create();

    float redAccumulator = 0;
    bool isRed = false;

    float deltaTime = 0;
    while (isRunning) {
        bool isPaused = game->isPaused;
        while (true) {

        // RUN ENGINE
        if (game->engine != NULL && !isPaused) {
            engine_tick(game->engine, deltaTime);
            if (game->engine->isDead) game->isPaused = true;
        }

        // DRAW GAME


        if (game->engine != NULL) {
        }

        // SWAP BUFFERS

    }
}}


void game_makeFullscreen(Game *game) {
        
}

void game_setupRenderer(Game *game) {
        // do nothing
}


