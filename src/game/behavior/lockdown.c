#include "lockdown.h"
#include "../../math/quick-math.h"

static void lockdown_resetVars(Lockdown *lockdown);

static bool lockdown_shouldLock(const Lockdown *lockdown, const ActivePiece *activePiece);


bool lockdown_update(Lockdown *lockdown, const ActivePiece *activePiece, const float deltaTime) {

    if (activePiece_collidesWith(activePiece, (CTetPoint) {0, -1})) {
        lockdown->lockDelayAcc += deltaTime;
    }

    const int pieceLastLowestY = lockdown->pieceLowestY;
    lockdown->pieceLowestY = MIN(lockdown->pieceLowestY, activePiece->pos.y);

    if (pieceLastLowestY > lockdown->pieceLowestY) {
        lockdown_resetVars(lockdown);
    }

    if (lockdown_shouldLock(lockdown, activePiece)) {
        return true;
    }
    return false;
}

void lockdown_onPieceSpawn(Lockdown *lockdown, const ActivePiece *activePiece) {
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

bool lockdown_shouldLock(const Lockdown *lockdown, const ActivePiece *activePiece) {
    return (lockdown->lockDelayAcc >= lockdown->lockDelay || lockdown->shouldForceLockPiece)
           && activePiece_collidesWith(activePiece, (CTetPoint) {0, -1});
}

float lockdown_lockDelayRemaining(const Lockdown *lockdown) {
    return 1 - lockdown->lockDelayAcc / lockdown->lockDelay;
}