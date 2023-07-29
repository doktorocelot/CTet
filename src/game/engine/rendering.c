#include "rendering.h"

#define COLOR_FLAG_CRITICAL_DANGER (1 << 0)
#define COLOR_FLAG_RED_FLASH_ON (1 << 1)
#define COLOR_FLAG_HOLD_IS_LOCKED (1 << 2)

#define RED_FLASH_LIMIT 75

#define DANGER_MULTIPLIER 0.5
#define HOLD_LOCKED_MULTIPLIER 0.4

typedef enum {
    DrawType_PIECE,
    DrawType_GHOST,
    DrawType_FIELD,
    DrawType_NEXT,
    DrawType_HOLD,
    DrawType_BORDER,
} DrawType;

static const int COLOR_VALUE_LUT[] = {
        255,
        100,
        200,
        255,
        255,
        255,
};

static const bool FLASH_IN_DANGER_LUT[] = {
        true,
        false,
        false,
        false,
        false,
        true,
};

static void rendering_sdlDrawBorder(SDL_Renderer *renderer);

static void rendering_sdlDrawPiece(SDL_Renderer *renderer, Piece *piece, Point offset);

static void rendering_sdlDrawField(SDL_Renderer *renderer, Block *matrix);

static void rendering_setSdlDrawColor(SDL_Renderer *renderer, DrawType type, int colorFlags);

static Point getPieceQueueOffset(const Piece *piece);

static bool shouldDrawRed(const DrawType *type, int colorFlags);

void rendering_tick(Rendering *vars, float deltaTime) {
    vars->redFlashAcc += deltaTime;
    if (vars->redFlashAcc >= RED_FLASH_LIMIT) {
        vars->redFlashAcc -= RED_FLASH_LIMIT;
        vars->redFlashOn ^= true;
    }
}

void rendering_drawEngineInSdl(Rendering *vars, SDL_Renderer *renderer, Engine *engine, bool isPaused) {

    // SETUP COLOR FLAGS
    int colorFlags = 0;
    colorFlags |= COLOR_FLAG_CRITICAL_DANGER * engine_placingPieceWillDie(engine);
    colorFlags |= COLOR_FLAG_RED_FLASH_ON * (vars->redFlashOn && !isPaused);
    colorFlags |= COLOR_FLAG_HOLD_IS_LOCKED * holdQueue_isLocked(&engine->holdQueue);

    // FIELD
    rendering_setSdlDrawColor(renderer, DrawType_FIELD, colorFlags);
    rendering_sdlDrawField(renderer, (Block *) engine->field.matrix);

    // BORDER
    rendering_setSdlDrawColor(renderer, DrawType_BORDER, colorFlags);
    rendering_sdlDrawBorder(renderer);

    // GHOST
    rendering_setSdlDrawColor(renderer, DrawType_GHOST, colorFlags);
    Point ghostPosition = (Point) {
            engine->active.pos.x,
            engine->active.pos.y - activePiece_getDistanceToGround(&engine->active),
    };
    rendering_sdlDrawPiece(renderer, &engine->active.piece, ghostPosition);

    // PIECE
    rendering_setSdlDrawColor(renderer, DrawType_PIECE, colorFlags);
    rendering_sdlDrawPiece(renderer, &engine->active.piece, engine->active.pos);

    // NEXT QUEUE
    rendering_setSdlDrawColor(renderer, DrawType_NEXT, colorFlags);
    Piece *nextPieces = engine->nextQueue.pieces;
    for (int i = 0; i < NEXT_QUEUE_LENGTH; i++) {
        Piece *piece = &nextPieces[i];
        Point additionalOffset = getPieceQueueOffset(piece);
        rendering_sdlDrawPiece(renderer, piece, point_addToNew((Point) {11, 16 - i * 4}, additionalOffset));
    }

    // HOLD QUEUE
    rendering_setSdlDrawColor(renderer, DrawType_HOLD, colorFlags);
    Piece *held = &engine->holdQueue.held;
    if (held->type != PieceType_NONE) {
        piece_resetOrientation(held);
        Point additionalOffset = getPieceQueueOffset(held);
        rendering_sdlDrawPiece(renderer, held, point_addToNew((Point) {-5, 16}, additionalOffset));
    }

    // PAUSED OVERLAY
    if (isPaused) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, &(SDL_Rect) {0, 0, GAME_ENGINE_WIDTH, GAME_ENGINE_HEIGHT});
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}

void rendering_sdlDrawBorder(SDL_Renderer *renderer) {
    SDL_RenderDrawRect(renderer,
                       &(SDL_Rect) {PADDING_H, PADDING_V, CELL_SIZE * FIELD_WIDTH, CELL_SIZE * FIELD_NORMAL_HEIGHT});
}

void rendering_sdlDrawPiece(SDL_Renderer *renderer, Piece *piece, Point offset) {
    for (int i = 0; i < 4; i++) {
        Point coords = piece->coords[i];
        SDL_RenderFillRect(renderer,
                           &(SDL_Rect) {
                                   (coords.x + offset.x) * CELL_SIZE + PADDING_H,
                                   CELL_SIZE * FIELD_NORMAL_HEIGHT - (coords.y + offset.y) * CELL_SIZE + PADDING_V -
                                   CELL_SIZE,
                                   CELL_SIZE,
                                   CELL_SIZE,
                           });

    }
}

void rendering_sdlDrawField(SDL_Renderer *renderer, Block *matrix) {
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

void rendering_setSdlDrawColor(SDL_Renderer *renderer, DrawType type, int colorFlags) {
    int colorValue = COLOR_VALUE_LUT[type];
    if (colorFlags & COLOR_FLAG_HOLD_IS_LOCKED && type == DrawType_HOLD) {
        colorValue = (int) (colorValue * HOLD_LOCKED_MULTIPLIER);
    }
    int r = colorValue;
    int g = colorValue;
    int b = colorValue;

    // Make parts flash red if game over is imminent
    if (shouldDrawRed(&type, colorFlags)) {
        g = (int) (g * DANGER_MULTIPLIER);
        b = g;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
}

bool shouldDrawRed(const DrawType *type, int colorFlags) {
    return FLASH_IN_DANGER_LUT[(*type)] &&
                colorFlags & COLOR_FLAG_RED_FLASH_ON &&
                colorFlags & COLOR_FLAG_CRITICAL_DANGER;
}
