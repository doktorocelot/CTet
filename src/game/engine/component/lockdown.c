#include "lockdown.h"

bool lockdown_tick(Lockdown *lockdown, ActivePiece *activePiece, float deltaTime) {
    if (activePiece_collidesWith(activePiece, (Point) {0, -1})) {
        lockdown->lockDelayAcc += deltaTime;
    } else {
        lockdown->lockDelayAcc = 0;
    }
    if (lockdown->lockDelayAcc >= LOCK_DELAY) {
        lockdown->lockDelayAcc -= LOCK_DELAY;
        return true;
    }
    return false;
}
