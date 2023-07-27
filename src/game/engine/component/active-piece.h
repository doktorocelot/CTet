#pragma once

#include <stdbool.h>
#include "../../../math/point.h"
#include "piece.h"
#include "field.h"

typedef struct {
    Point pos;
    Piece piece;
    Field *field;
} ActivePiece;

typedef enum {
    ShiftDirection_LEFT = -1,
    ShiftDirection_NONE,
    ShiftDirection_RIGHT,
} ShiftDirection;

bool activePiece_newPiece(ActivePiece *active, Piece piece);

bool activePiece_shift(ActivePiece *active, ShiftDirection direction);

bool activePiece_dropOneLine(ActivePiece *active);

bool activePiece_collidesWithOrientation(ActivePiece *active, Orientation orientation, Point offset);

bool activePiece_collidesWith(ActivePiece *active, Point offset);

void activePiece_slamToFloor(ActivePiece *active);

int activePiece_getDistanceToGround(ActivePiece *active);

void activePiece_placeToField(ActivePiece *active);

bool activePiece_rotate(ActivePiece *active, int amount);