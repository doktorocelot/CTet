#pragma once

#include <stdint.h>
#include "piece.h"

#define NEXT_QUEUE_LENGTH 5

typedef struct {
    Piece pieces[5];
    uint64_t nextSeed;
    PieceType bag[PieceType_LENGTH];
    int remainingPiecesInBag;
} NextQueue;

void nextQueue_reset(NextQueue *nextQueue);

Piece nextQueue_next(NextQueue *nextQueue);

Piece nextQueue_peek(NextQueue *nextQueue);