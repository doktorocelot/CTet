#pragma once

#include <stdbool.h>
#include "active-piece.h"

typedef struct {
    float msPerRow;
    float gravityAccumulator;
    bool softDropIsDown;
} Gravity;

int gravity_update(Gravity *gravity, ActivePiece *activePiece, float deltaTime);

void gravity_onHitFloor(Gravity *gravity);