#include "gravity.h"

int gravity_tick(Gravity *gravity, float deltaTime) {
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
