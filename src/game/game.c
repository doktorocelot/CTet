#include <SDL.h>
#include <stdio.h>
#include "game.h"
#include "stdbool.h"
#include "engine/component/piece.h"
#include "engine/engine.h"
#include "engine/component/field.h"
#include "engine/component/next-queue.h"

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
    bool isPaused;
};

#define GAME_ENGINE_WIDTH 720
#define GAME_ENGINE_HEIGHT 720

static int CELL_SIZE;
static int PADDING_H;
static int PADDING_V;

static void game_setupRenderer(Game *game);

static void game_makeFullscreen(Game *game);

static void game_renderer_drawPiece(SDL_Renderer *renderer, Piece *piece, Point offset);

static void game_renderer_drawBoard(SDL_Renderer *renderer);

static void game_renderer_drawField(SDL_Renderer *renderer, Block *matrix);

static int game_getDrawColor(Game *game);

Point getPieceQueueOffset(const Piece *piece);

Game *game_create() {
    Game *game = malloc(sizeof(Game));

    SDL_Init(SDL_INIT_VIDEO);

    CELL_SIZE = (int) (GAME_ENGINE_HEIGHT / 22.5);
    PADDING_H = (GAME_ENGINE_WIDTH - CELL_SIZE * FIELD_WIDTH) / 2;
    PADDING_V = (GAME_ENGINE_HEIGHT - CELL_SIZE * FIELD_NORMAL_HEIGHT) / 2;

    game->window = SDL_CreateWindow(
            "CTet",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            GAME_ENGINE_WIDTH, GAME_ENGINE_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    game->sdlRenderer = SDL_CreateRenderer(
            game->window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    game->engine = NULL;

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
                        if (engine_isDead(game->engine)) break;
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


        // sdl delta time
        Uint64 currentTime = SDL_GetTicks();
        float deltaTime = (float) currentTime - (float) previousTime;
        previousTime = currentTime;
        if (engine_placingPieceWillDie(game->engine) && !engine_isDead(game->engine)) {
            redAccumulator += deltaTime;
            int redCheck = isRed ? 75 : 50;
            if (redAccumulator > (float) redCheck) {
                isRed ^= true;
                redAccumulator -= (float) redCheck;
            }
        } else {
            isRed = false;
            redAccumulator = 0;
        }


        //engine
        if (game->engine != NULL && !isPaused) {
            engine_tick(game->engine, deltaTime);
            if (engine_isDead(game->engine)) game->isPaused = true;
        }

        // draw
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int drawColor = game_getDrawColor(game);
        float range = 2.0f;
        int drawColorPiece = (int) (
                ((LOCK_DELAY - engine_getLockdown(game->engine)->lockDelayAcc) / (range * LOCK_DELAY) +
                 1 / range * (range - 1)) *
                (float) drawColor);
        int drawColorStack = (int) (drawColor / 1.2);
        int drawColorGhost = (int) (drawColor / 2.2);


        if (game->engine != NULL) {
            //stack
            SDL_SetRenderDrawColor(renderer, drawColorStack, drawColorStack, drawColorStack, 255);
            game_renderer_drawField(renderer, engine_getFieldMatrix(game->engine));

            //border
            SDL_SetRenderDrawColor(renderer, drawColor, isRed ? 100 : drawColor, isRed ? 100 : drawColor, 255);
            game_renderer_drawBoard(renderer);


            Point activePiecePos = *engine_getActivePiecePos(game->engine);
            Piece *activePiece = engine_getActivePiece(game->engine);

            //ghost
            SDL_SetRenderDrawColor(renderer, drawColorGhost, drawColorGhost, drawColorGhost, 255);
            game_renderer_drawPiece(renderer, activePiece,
                                    (Point) {activePiecePos.x, activePiecePos.y -
                                                               engine_getDistanceFromActivePieceToGround(
                                                                       game->engine)});

            //active
            SDL_SetRenderDrawColor(renderer, drawColorPiece, isRed ? (int) (drawColorPiece * 0.5) : drawColorPiece, isRed ? (int) (drawColorPiece * 0.5) : drawColorPiece, 255);
            game_renderer_drawPiece(renderer, activePiece,activePiecePos);

            //next
            SDL_SetRenderDrawColor(renderer, drawColor, drawColor, drawColor, 255);
            Piece *nextPieces = engine_getNextPieces(game->engine);
            for (int i = 0; i < NEXT_QUEUE_LENGTH; i++) {
                Piece *piece = &nextPieces[i];
                Point additionalOffset = getPieceQueueOffset(piece);
                game_renderer_drawPiece(renderer, piece, point_addToNew((Point) {11, 16 - i * 4}, additionalOffset));
            }

            //held
            Piece *held = engine_getHeldPiece(game->engine);
            if (held->type != PieceType_NONE) {
                piece_resetOrientation(held);
                Point additionalOffset = getPieceQueueOffset(held);
                game_renderer_drawPiece(renderer, held, point_addToNew((Point) {-5, 16}, additionalOffset));
            }
        }



        SDL_RenderPresent(renderer);
    }
}

Point getPieceQueueOffset(const Piece *piece) {
    Point additionalOffset;
    switch (piece->type) {
        case PieceType_O:
            additionalOffset = (Point) {1, 1};
            break;
        default:
            additionalOffset = (Point) {0};
            break;
    }
    return additionalOffset;
}

void game_makeFullscreen(Game *game) {
    Uint32 fullscreenFlag = SDL_GetWindowFlags(game->window) & SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_SetWindowFullscreen(game->window, fullscreenFlag ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void game_setupRenderer(Game *game) {
    SDL_Renderer *renderer = game->sdlRenderer;

    SDL_RenderSetLogicalSize(renderer, GAME_ENGINE_WIDTH, GAME_ENGINE_HEIGHT);
}

void game_renderer_drawPiece(SDL_Renderer *renderer, Piece *piece, Point offset) {
    for (int i = 0; i < 4; i++) {
        Point coords = piece->coords[i];
        SDL_RenderFillRect(renderer,
                           &(SDL_Rect) {
                                   (coords.x + offset.x) * CELL_SIZE + PADDING_H,
                                   CELL_SIZE * FIELD_NORMAL_HEIGHT - (coords.y + offset.y) * CELL_SIZE + PADDING_V - CELL_SIZE,
                                   CELL_SIZE,
                                   CELL_SIZE,
                           });

    }
}


void game_renderer_drawBoard(SDL_Renderer *renderer) {
    SDL_RenderDrawRect(renderer, &(SDL_Rect) {PADDING_H, PADDING_V, CELL_SIZE * FIELD_WIDTH, CELL_SIZE * FIELD_NORMAL_HEIGHT});
}

void game_renderer_drawField(SDL_Renderer *renderer, Block *matrix) {
    for (int y = 0; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++) {
            int index = FIELD_WIDTH * y + x;
            if (matrix[index].color <= BlockColor_NONE) continue;
            SDL_RenderFillRect(renderer,
                               &(SDL_Rect) {
                                       x * CELL_SIZE + PADDING_H,
                                       CELL_SIZE * 20 - y * CELL_SIZE - CELL_SIZE + PADDING_V,
                                       CELL_SIZE,
                                       CELL_SIZE
                               });
        }
    }
}

int game_getDrawColor(Game *game) {
    return game->isPaused ? 150 : 255;
}
