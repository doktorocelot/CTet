#pragma once

#include "block.h"
#include "../../../math/point.h"

#define BLOCKS_PER_PIECE 4

// Arranged alphabetically
typedef enum {
    PieceType_NONE = -1,
    PieceType_I,
    PieceType_J,
    PieceType_L,
    PieceType_O,
    PieceType_S,
    PieceType_T,
    PieceType_Z,
    PieceType_LENGTH,
} PieceType;

typedef enum {
    Orientation_UP,
    Orientation_RIGHT,
    Orientation_DOWN,
    Orientation_LEFT,
    Orientation_LENGTH,
} Orientation;

#define ROTATION_CW 1
#define ROTATION_CCW 3
#define ROTATION_180 2

typedef struct { // Do not modify the struct directly
    Block blocks[BLOCKS_PER_PIECE];
    Point *coords;
    Orientation orientation;
    PieceType type;
} Piece;

Piece piece_buildFromType(PieceType type);

void piece_rotate(Piece *piece, int amount);

void piece_resetOrientation(Piece *piece);

Orientation piece_getNewOrientation(const Piece *piece, int amount);

Point *piece_getCoordsForOrientation(Piece *piece, Orientation orientation);