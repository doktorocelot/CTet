#include <malloc.h>
#include <time.h>
#include <ctet/ctet.h>
#include "../math/prng.h"
#include "data/piece-data.h"

#include <ctet/ctet.h>

#include "component/active-piece.h"
#include "component/lockdown.h"
#include "component/hold-queue.h"
#include "component/next-queue.h"
#include "component/gravity.h"
#include "component/autoshift.h"

struct CTetEngine {
    Field field;
    AutoshiftVars autoshiftVars;
    Gravity gravity;
    NextQueue nextQueue;
    HoldQueue holdQueue;
    Lockdown lockdown;
    ActivePiece active;
    bool isDead;
};

static int retryCount = 0;

static void spawnNextPiece(CTetEngine *engine, CTetPiece piece);

static void lockdown(CTetEngine *engine);

static void shiftActive(CTetEngine *engine, ShiftDirection dir);

static void rotateActive(CTetEngine *engine, int amount);

CTetEngine *ctEngine_create() {
    CTetEngine *engine = malloc(sizeof(CTetEngine));

    engine->active.field = &engine->field;

    ctEngine_reset(engine);

    return engine;
}

void ctEngine_destroy(CTetEngine *engine) {
    free(engine);
}

void ctEngine_reset(CTetEngine *engine) {
    // Setup Field
    field_clear(&engine->field);

    // Setup Autoshift
    engine->autoshiftVars = (AutoshiftVars) {0};

    // Setup Gravity
    engine->gravity = (Gravity) {.msPerRow = 1000};

    // Setup NEXT
    engine->nextQueue = (NextQueue) {.nextSeed = (time(NULL) + retryCount * (retryCount % 10))};
    retryCount++;
    nextQueue_reset(&engine->nextQueue);

    // Setup HOLD
    holdQueue_reset(&engine->holdQueue);

    // Setup Lockdown
    engine->lockdown = (Lockdown) {0};

    // Setup ActivePiece
    spawnNextPiece(engine, nextQueue_next(&engine->nextQueue));

    // Make engine not dead
    engine->isDead = false;
}

void ctEngine_update(CTetEngine *engine, float deltaMillis) {

    int autoshiftResult;
    while (autoshiftResult = autoshift_tick(&engine->autoshiftVars, deltaMillis), autoshiftResult) {
        shiftActive(engine, autoshiftResult);
    }

    int gravityResult;
    while (gravityResult = gravity_tick(&engine->gravity, &engine->active, deltaMillis), gravityResult) {
        if (!activePiece_dropOneLine(&engine->active)) {
            gravity_onHitFloor(&engine->gravity);
        }
    }

    if (lockdown_tick(&engine->lockdown, &engine->active, deltaMillis)) {
        lockdown(engine);
    }
}

void ctEngine_onShiftRightDown(CTetEngine *engine) {
    shiftActive(engine, ShiftDirection_RIGHT);
    autoshift_onPress(&engine->autoshiftVars, ShiftDirection_RIGHT);
}

void ctEngine_onShiftRightUp(CTetEngine *engine) {
    autoshift_onRelease(&engine->autoshiftVars, ShiftDirection_RIGHT);
}

void ctEngine_onShiftLeftDown(CTetEngine *engine) {
    shiftActive(engine, ShiftDirection_LEFT);
    autoshift_onPress(&engine->autoshiftVars, ShiftDirection_LEFT);
}

void ctEngine_onShiftLeftUp(CTetEngine *engine) {
    autoshift_onRelease(&engine->autoshiftVars, ShiftDirection_LEFT);
}

void ctEngine_onHardDrop(CTetEngine *engine) {
    activePiece_slamToFloor(&engine->active);
    lockdown(engine);
}

void lockdown(CTetEngine *engine) {
    activePiece_placeToField(&engine->active);
    holdQueue_onLock(&engine->holdQueue);

    int hitList[CT_TOTAL_FIELD_HEIGHT + 1];
    field_getFullRowHitList(&engine->field, hitList);
    field_killHitList(&engine->field, hitList);
    field_collapseHitList(&engine->field, hitList);

    spawnNextPiece(engine, nextQueue_next(&engine->nextQueue));
}

void ctEngine_onRotateLeft(CTetEngine *engine) {
    rotateActive(engine, CT_ROTATION_CCW);
}

void ctEngine_onRotateRight(CTetEngine *engine) {
    rotateActive(engine, CT_ROTATION_CW);
}

void ctEngine_onSoftDropDown(CTetEngine *engine) {
    engine->gravity.softDropIsDown = true;
}

void ctEngine_onSoftDropUp(CTetEngine *engine) {
    engine->gravity.softDropIsDown = false;
}

void ctEngine_onHoldDown(CTetEngine *engine) {
    CTetPiece holdReturn;
    if (holdQueue_performHold(&engine->holdQueue, &holdReturn, &engine->active.piece)) {
        if (holdReturn.type == CTetPieceType_NONE) spawnNextPiece(engine, nextQueue_next(&engine->nextQueue));
        else spawnNextPiece(engine, holdReturn);
    }

}

CTetPoint ctEngine_getActivePiecePos(CTetEngine *engine) {
    return engine->active.pos;
}

CTetPoint ctEngine_getGhostOffset(CTetEngine *engine) {
    return (CTetPoint) {0, -activePiece_getDistanceToGround(&engine->active)};
}

const CTetPiece *ctEngine_getActivePiece(CTetEngine *engine) {
    return &engine->active.piece;
}

float ctEngine_getLockDelayRemainingPercentage(CTetEngine *engine) {
    return lockdown_lockDelayRemaining(&engine->lockdown);
}

const CTetPiece *ctEngine_getNextPieces(CTetEngine *engine) {
    return engine->nextQueue.pieces;
}

const CTetPiece *ctEngine_getHeldPiece(CTetEngine *engine) {
    return &engine->holdQueue.held;
}

const CTetBlock *ctEngine_getBlockAtFieldLocation(CTetEngine *engine, CTetPoint location) {
    return &engine->field.matrix[location.y][location.x];
}

void spawnNextPiece(CTetEngine *engine, CTetPiece piece) {
    if (!activePiece_newPiece(&engine->active, piece)) {
        engine->isDead = true;
    }
    lockdown_onPieceSpawn(&engine->lockdown, &engine->active);
}

void shiftActive(CTetEngine *engine, ShiftDirection dir) {
    if (activePiece_shift(&engine->active, dir)) {
        lockdown_onPieceManipulate(&engine->lockdown);
    }
}

void rotateActive(CTetEngine *engine, int amount) {
    if (activePiece_rotate(&engine->active, amount)) {
        lockdown_onPieceManipulate(&engine->lockdown);
    }
}
