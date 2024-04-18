#include "game/behavior/behavior.h"

int gravity_update(Gravity *gravity, ActivePiece *activePiece, const float deltaTime) {
    if (activePiece_collidesWith(activePiece, (CTetPoint) {0, -1})) {
        gravity_onHitFloor(gravity);
        return 0;
    }
    if (gravity->msPerRow < 0) {
        activePiece_slamToFloor(activePiece, NULL);
        return 0;
    }
    if (gravity->gravityAccumulator >= 1) {
        gravity->gravityAccumulator -= 1;
        return 1;
    }
    const int softDropMultiplier = gravity->softDropIsDown ? 20 : 1;
    gravity->gravityAccumulator += deltaTime / gravity->msPerRow * (float) softDropMultiplier;
    return 0;
}

void gravity_onHitFloor(Gravity *gravity) {
    gravity->gravityAccumulator = 0;
}
