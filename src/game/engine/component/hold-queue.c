#include <ctet/hold-queue.h>

void holdQueue_reset(HoldQueue *holdQueue) {
    holdQueue->held = (Piece) {.type = PieceType_NONE};
    holdQueue->holdCountBeforeLock = 0;
}

bool holdQueue_performHold(HoldQueue *holdQueue, Piece *dest, Piece *src) {
    if (holdQueue_isLocked(holdQueue)) return false;
    *dest = holdQueue->held;
    holdQueue->held = *src;
    holdQueue->holdCountBeforeLock++;
    return true;
}

void holdQueue_onLock(HoldQueue *holdQueue) {
    holdQueue->holdCountBeforeLock = 0;
}

bool holdQueue_isLocked(HoldQueue *holdQueue) {
    return holdQueue->holdCountBeforeLock >= 1;
}
