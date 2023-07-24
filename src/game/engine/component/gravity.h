#pragma once

#include <stdbool.h>

typedef struct {
    float msPerRow;
    float gravityAccumulator;
    bool softDropIsDown;
} Gravity;

int gravity_tick(Gravity *gravity, float deltaTime);

void gravity_onHitFloor(Gravity *gravity);