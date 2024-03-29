#pragma once

#include <stdbool.h>
#include "../component/active-piece.h"

#define MANIPULATION_LIMIT 15

typedef struct {
    float lockDelay;
    float lockDelayAcc;
    int pieceLowestY;
    int pieceManipulations;
    bool shouldForceLockPiece;
} Lockdown;

bool lockdown_update(Lockdown *lockdown, const ActivePiece *activePiece, float deltaTime);

void lockdown_onPieceSpawn(Lockdown *lockdown, const ActivePiece *activePiece);

void lockdown_onPieceManipulate(Lockdown *lockdown);

float lockdown_lockDelayRemaining(const Lockdown *lockdown);