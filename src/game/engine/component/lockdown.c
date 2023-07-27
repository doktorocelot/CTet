#include "lockdown.h"
#include "../../../math/quick-math.h"

bool lockdown_tick(Lockdown *lockdown, ActivePiece *activePiece, float deltaTime) {

    if (activePiece_collidesWith(activePiece, (Point) {0, -1})) {
        lockdown->lockDelayAcc += deltaTime;
    }

    int pieceLastLowestY = lockdown->pieceLowestY;
    lockdown->pieceLowestY = MIN(lockdown->pieceLowestY, activePiece->pos.y);

    if (pieceLastLowestY > lockdown->pieceLowestY) {
        lockdown->lockDelayAcc = 0;
    }

    if (lockdown->lockDelayAcc >= LOCK_DELAY) {
        lockdown->lockDelayAcc -= LOCK_DELAY;
        return true;
    }
    return false;
}

void lockdown_onPieceSpawn(Lockdown *lockdown, ActivePiece *activePiece) {
    lockdown->lockDelayAcc = 0;
    lockdown->pieceLowestY = activePiece->pos.y;
}

void lockdown_onPieceManipulate(Lockdown *lockdown) {
    lockdown->lockDelayAcc = 0;
}
