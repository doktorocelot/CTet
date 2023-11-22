#include "hold-queue.h"

void holdQueue_reset(HoldQueue *holdQueue) {
    holdQueue->held = (CTetPiece) {.type = CTetPieceType_NONE};
    holdQueue->holdCountBeforeLock = 0;
}

bool holdQueue_performHold(HoldQueue *holdQueue, CTetPiece *dest, const CTetPiece *src) {
    if (holdQueue_isLocked(holdQueue)) return false;
    *dest = holdQueue->held;
    holdQueue->held = *src;
    holdQueue->holdCountBeforeLock++;
    piece_resetOrientation(&holdQueue->held);
    return true;
}

void holdQueue_onLock(HoldQueue *holdQueue) {
    holdQueue->holdCountBeforeLock = 0;
}

bool holdQueue_isLocked(const HoldQueue *holdQueue) {
    return holdQueue->holdCountBeforeLock >= 1;
}
