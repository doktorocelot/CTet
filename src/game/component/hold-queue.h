#pragma once

#include <stdbool.h>
#include "piece.h"

typedef struct {
    CTetPiece held;
    int holdCountBeforeLock;
} HoldQueue;

void holdQueue_reset(HoldQueue *holdQueue);

bool holdQueue_performHold(HoldQueue *holdQueue, CTetPiece *dest, CTetPiece *src);

void holdQueue_onLock(HoldQueue *holdQueue);

bool holdQueue_isLocked(HoldQueue *holdQueue);