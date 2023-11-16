#pragma once

#include <stdbool.h>
#include "active-piece.h"

#define LOCK_DELAY 500
#define MANIPULATION_LIMIT 15

typedef struct {
    float lockDelayAcc;
    int pieceLowestY;
    int pieceManipulations;
    bool shouldForceLockPiece;
} Lockdown;

bool lockdown_update(Lockdown *lockdown, ActivePiece *activePiece, float deltaTime);

void lockdown_onPieceSpawn(Lockdown *lockdown, ActivePiece *activePiece);

void lockdown_onPieceManipulate(Lockdown *lockdown);

float lockdown_lockDelayRemaining(Lockdown *lockdown);