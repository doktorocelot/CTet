#include <ctet/gravity.h>

int gravity_tick(Gravity *gravity, ActivePiece *activePiece, float deltaTime) {
    if (activePiece_collidesWith(activePiece, (Point) {0, -1})) {
        gravity_onHitFloor(gravity);
        return 0;
    } else if (gravity->msPerRow < 0) {
        activePiece_slamToFloor(activePiece);
        return 0;
    }
    if (gravity->gravityAccumulator >= 1) {
        gravity->gravityAccumulator -= 1;
        return 1;
    }
    int softDropMultiplier = gravity->softDropIsDown ? 20 : 1;
    gravity->gravityAccumulator += (deltaTime / gravity->msPerRow) * (float) softDropMultiplier;
    return 0;
}

void gravity_onHitFloor(Gravity *gravity) {
    gravity->gravityAccumulator = 0;
}
