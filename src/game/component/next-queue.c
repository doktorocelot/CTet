#include <memory.h>
#include "next-queue.h"
#include "../../math/prng.h"

void nextQueue_genNewPieceAt(NextQueue *nextQueue, int i);

void nextQueue_repopulateBag(NextQueue *nextQueue);

void nextQueue_reset(NextQueue *nextQueue) {
    nextQueue->remainingPiecesInBag = 0;
    cycleSeed(&nextQueue->nextSeed);
    for (int i = 0; i < CT_NEXT_QUEUE_MAX_LENGTH; i++) {
        nextQueue_genNewPieceAt(nextQueue, i);
        cycleSeed(&nextQueue->nextSeed);
    }
}

CTetPiece nextQueue_next(NextQueue *nextQueue) { // Todo optimize with a cycling array and add iterator for drawing
    CTetPiece result = nextQueue->pieces[0];
    memmove(nextQueue->pieces, nextQueue->pieces + 1, (CT_NEXT_QUEUE_MAX_LENGTH - 1) * sizeof(CTetPiece));
    cycleSeed(&nextQueue->nextSeed);
    nextQueue_genNewPieceAt(nextQueue, CT_NEXT_QUEUE_MAX_LENGTH - 1);
    return result;
}

CTetPiece nextQueue_peek(NextQueue *nextQueue) {
    return nextQueue->pieces[0];
}

void nextQueue_genNewPieceAt(NextQueue *nextQueue, int i) {
    uint64_t nextSeed = nextQueue->nextSeed;

    if (nextQueue->remainingPiecesInBag == 0) nextQueue_repopulateBag(nextQueue);

    unsigned int selectedIndex = (unsigned int) nextSeed % nextQueue->remainingPiecesInBag;

    nextQueue->pieces[i] = piece_buildFromType(nextQueue->bag[selectedIndex]);

    if (selectedIndex != nextQueue->remainingPiecesInBag - 1) {
        *(nextQueue->bag + selectedIndex) = nextQueue->bag[nextQueue->remainingPiecesInBag - 1];
    }
    nextQueue->remainingPiecesInBag--;

}

void nextQueue_repopulateBag(NextQueue *nextQueue) {
    for (int i = 0; i < CTetPieceType_LENGTH; i++) {
        nextQueue->bag[i] = i;
    }
    nextQueue->remainingPiecesInBag = CTetPieceType_LENGTH;
}
