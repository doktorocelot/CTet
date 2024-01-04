#pragma once

#include <stdbool.h>
#include <ctet/ctet.h>
#include "field.h"

typedef struct {
    CTetPoint pos;
    CTetPiece piece;
    Field *field;
} ActivePiece;

typedef enum {
    ShiftDirection_LEFT = -1,
    ShiftDirection_NONE,
    ShiftDirection_RIGHT,
} ShiftDirection;

void activePiece_newPiece(ActivePiece *active, CTetPiece piece);

bool activePiece_shift(ActivePiece *active, ShiftDirection direction);

bool activePiece_dropOneLine(ActivePiece *active);

bool activePiece_collidesWithOrientation(const ActivePiece *active, CTetOrientation orientation, CTetPoint offset);

bool activePiece_collidesWith(const ActivePiece *active, CTetPoint offset);

void activePiece_slamToFloor(ActivePiece *active, int *distanceOut);

int activePiece_getDistanceToGround(const ActivePiece *active);

void activePiece_placeToField(ActivePiece *active, double lockTimestamp);

bool activePiece_rotate(ActivePiece *active, int amount);