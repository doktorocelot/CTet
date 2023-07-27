#include "lockdown.h"
#include "../../../math/quick-math.h"

static void lockdown_resetVars(Lockdown *lockdown);

static bool lockdown_shouldLock(const Lockdown *lockdown, ActivePiece *activePiece);


bool lockdown_tick(Lockdown *lockdown, ActivePiece *activePiece, float deltaTime) {

    if (activePiece_collidesWith(activePiece, (Point) {0, -1})) {
        lockdown->lockDelayAcc += deltaTime;
    }

    int pieceLastLowestY = lockdown->pieceLowestY;
    lockdown->pieceLowestY = MIN(lockdown->pieceLowestY, activePiece->pos.y);

    if (pieceLastLowestY > lockdown->pieceLowestY) {
        lockdown_resetVars(lockdown);
    }

    if (lockdown_shouldLock(lockdown, activePiece)) {
        return true;
    }
    return false;
}

void lockdown_onPieceSpawn(Lockdown *lockdown, ActivePiece *activePiece) {
    lockdown_resetVars(lockdown);
    lockdown->pieceLowestY = activePiece->pos.y;
}

void lockdown_onPieceManipulate(Lockdown *lockdown) {
    lockdown->pieceManipulations++;
    if (lockdown->pieceManipulations > MANIPULATION_LIMIT) {
        lockdown->shouldForceLockPiece = true;
    } else {
        lockdown->lockDelayAcc = 0;
    }
}

void lockdown_resetVars(Lockdown *lockdown) {
    lockdown->lockDelayAcc = 0;
    lockdown->pieceManipulations = 0;
    lockdown->shouldForceLockPiece = false;
}

bool lockdown_shouldLock(const Lockdown *lockdown, ActivePiece *activePiece) {
    return (lockdown->lockDelayAcc >= LOCK_DELAY || lockdown->shouldForceLockPiece)
           && activePiece_collidesWith(activePiece, (Point) {0, -1});
}