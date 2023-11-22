#pragma once

#include <stdint.h>
#include <ctet/ctet.h>


typedef struct {
    CTetPiece pieces[5];
    uint64_t nextSeed;
    CTetPieceType bag[CTetPieceType_LENGTH];
    int remainingPiecesInBag;
} NextQueue;

void nextQueue_reset(NextQueue *nextQueue);

CTetPiece nextQueue_next(NextQueue *nextQueue);

