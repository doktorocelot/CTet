#pragma once

#include <SDL_render.h>

#include "engine.h"

#define CELL_SIZE 32

#define GAME_ENGINE_WIDTH (CELL_SIZE * 24)
#define GAME_ENGINE_HEIGHT (CELL_SIZE * 24)

#define PADDING_H ((GAME_ENGINE_WIDTH - CELL_SIZE * FIELD_WIDTH) / 2)
#define PADDING_V ((GAME_ENGINE_HEIGHT - CELL_SIZE * FIELD_NORMAL_HEIGHT) / 2)

typedef struct {
    float redFlashAcc;
    bool redFlashOn;

} Rendering;

void rendering_tick(Rendering *vars, float deltaTime);

void rendering_drawEngineInSdl(Rendering *vars, SDL_Renderer *renderer, Engine *engine, bool isPaused);