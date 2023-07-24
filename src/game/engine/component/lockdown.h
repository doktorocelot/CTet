#pragma once

#include <stdbool.h>
#include "active-piece.h"

#define LOCK_DELAY 500

typedef struct {
    float lockDelayAcc;
} Lockdown;

bool lockdown_tick(Lockdown *lockdown, ActivePiece *activePiece, float deltaTime);