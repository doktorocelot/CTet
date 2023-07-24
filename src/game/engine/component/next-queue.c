#include <memory.h>
#include "next-queue.h"
#include "../../../math/prng.h"

void nextQueue_genNewPieceAt(NextQueue *nextQueue, int i);

void nextQueue_reset(NextQueue *nextQueue) {
    cycleSeed(&nextQueue->nextSeed);
    for (int i = 0; i < NEXT_QUEUE_LENGTH; i++) {
        nextQueue_genNewPieceAt(nextQueue, i);
        cycleSeed(&nextQueue->nextSeed);
    }
}

Piece nextQueue_next(NextQueue *nextQueue) { // Todo optimize with a cycling array and add iterator for drawing
    Piece result = nextQueue->pieces[0];
    memmove(nextQueue->pieces, nextQueue->pieces + 1, (NEXT_QUEUE_LENGTH - 1) * sizeof(Piece));
    cycleSeed(&nextQueue->nextSeed);
    nextQueue_genNewPieceAt(nextQueue, NEXT_QUEUE_LENGTH - 1);
    return result;
}

void nextQueue_genNewPieceAt(NextQueue *nextQueue, int i) {
    nextQueue->pieces[i] = piece_buildFromType(nextQueue->nextSeed % PieceType_LENGTH);
}
