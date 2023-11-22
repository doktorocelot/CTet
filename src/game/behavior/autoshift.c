#include "autoshift.h"

#define AUTOSHIFT_DELAY 200
#define AUTOSHIFT_RATE 33

int autoshift_update(AutoshiftVars *vars, const float delta) {
    if (vars->currentDir == 0) return 0;

    if (vars->hasReleasedShift) {
        vars->hasReleasedShift = false;
        return vars->currentDir;
    }
    if (vars->autoshiftDelayAcc < AUTOSHIFT_DELAY) {
        vars->autoshiftDelayAcc += delta;
        return 0;
    }
    if (vars->autoshiftDelayAcc >= AUTOSHIFT_DELAY) {

        const bool shouldShift = vars->autoshiftRateAcc >= AUTOSHIFT_RATE;
        if (shouldShift) {
            vars->autoshiftRateAcc -= AUTOSHIFT_RATE;
            return vars->currentDir;
        }
        vars->autoshiftRateAcc += delta;
    }

    return 0;
}

void autoshift_onPress(AutoshiftVars *vars, const int direction) {
    if (direction == -1) {
        vars->leftIsDown = true;
    } else {
        vars->rightIsDown = true;
    }
    vars->autoshiftDelayAcc = 0;
    vars->autoshiftRateAcc = AUTOSHIFT_RATE;
    vars->currentDir = direction;
}

void autoshift_onRelease(AutoshiftVars *vars, const int direction) {
    if (direction == -1) {
        vars->leftIsDown = false;
    } else {
        vars->rightIsDown = false;
    }
    if (direction == -1 ? vars->rightIsDown == true : vars->leftIsDown == true) {
        if (vars->currentDir == direction) {
            vars->autoshiftDelayAcc = 0;
            vars->autoshiftRateAcc = AUTOSHIFT_RATE;
            vars->hasReleasedShift = true;
        }

        vars->currentDir = direction * -1;

    } else {
        vars->autoshiftDelayAcc = 0;
        vars->autoshiftRateAcc = AUTOSHIFT_RATE;
        vars->currentDir = 0;
    }
}
