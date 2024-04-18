#pragma once

#include "game/component/component.h"
#include <stdbool.h>

/** Autoshift **/

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


/** Gravity **/

typedef struct {
    float msPerRow;
    float gravityAccumulator;
    bool softDropIsDown;
} Gravity;

int gravity_update(Gravity *gravity, ActivePiece *activePiece, float deltaTime);

void gravity_onHitFloor(Gravity *gravity);


/** Lockdown **/

#define MANIPULATION_LIMIT 15

typedef struct {
    float lockDelay;
    float lockDelayAcc;
    int pieceLowestY;
    int pieceManipulations;
    bool shouldForceLockPiece;
} Lockdown;

bool lockdown_update(Lockdown *lockdown, const ActivePiece *activePiece, float deltaTime);

void lockdown_onPieceSpawn(Lockdown *lockdown, const ActivePiece *activePiece);

void lockdown_onPieceManipulate(Lockdown *lockdown);

float lockdown_lockDelayRemaining(const Lockdown *lockdown);


/** Wallkick **/

bool executeWallkick(CTetPoint *dest, const ActivePiece *active, int direction);