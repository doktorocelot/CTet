#pragma once

#include <stdbool.h>

typedef struct {
    float autoshiftDelayAcc;
    float autoshiftRateAcc;
    bool hasReleasedShift;
    int currentDir;
    bool rightIsDown;
    bool leftIsDown;
} AutoshiftVars;

int autoshift_update(AutoshiftVars *vars, float delta);

void autoshift_onPress(AutoshiftVars *vars, int direction);
void autoshift_onRelease(AutoshiftVars *vars, int direction);