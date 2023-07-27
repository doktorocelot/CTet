#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "engine.h"
#include "component/active-piece.h"
#include "../../math/prng.h"
#include "component/autoshift.h"
#include "component/gravity.h"
#include "component/next-queue.h"
#include "component/hold-queue.h"

static int retryCount = 0;

struct Engine {
    Field field;
    AutoshiftVars autoshiftVars;
    Gravity gravity;
    NextQueue nextQueue;
    HoldQueue holdQueue;
    Lockdown lockdown;
    ActivePiece active;
};


static void engine_lock(Engine *engine);

static void engine_shiftActive(Engine *engine, ShiftDirection dir);

static void engine_rotateActive(Engine *engine, int amount);

Engine *engine_create() {
    Engine *engine = malloc(sizeof(Engine));

    engine->active.field = &engine->field;

    engine_reset(engine);

    return engine;
}

void engine_destroy(Engine *engine) {
    free(engine);
}

void engine_reset(Engine *engine) {
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
    engine_spawnNewPiece(engine);
}

void engine_tick(Engine *engine, float deltaTime) {

    int autoshiftResult;
    while (autoshiftResult = autoshift_tick(&engine->autoshiftVars, deltaTime), autoshiftResult) {
        activePiece_shift(&engine->active, autoshiftResult);
    };

    int gravityResult;
    while (gravityResult = gravity_tick(&engine->gravity, &engine->active, deltaTime), gravityResult) {
        if (!activePiece_dropOneLine(&engine->active)) {
            gravity_onHitFloor(&engine->gravity);
        }
    }

    if (lockdown_tick(&engine->lockdown, &engine->active, deltaTime)) {
        engine_lock(engine);
    }

    int hitList[FIELD_HEIGHT + 1];
    field_getFullRowHitList(&engine->field, hitList);
    field_killHitList(&engine->field, hitList);
    field_collapseHitList(&engine->field, hitList);
}

Piece *engine_getActivePiece(Engine *engine) {
    return &engine->active.piece;
}

Point *engine_getActivePiecePos(Engine *engine) {
    return &engine->active.pos;
}

int engine_getDistanceFromActivePieceToGround(Engine *engine) {
    return activePiece_getDistanceToGround(&engine->active);
}

Block *engine_getFieldMatrix(Engine *engine) {
    return (Block *) engine->field.matrix;
}

Piece *engine_getNextPieces(Engine *engine) {
    return (Piece *) engine->nextQueue.pieces;
}

Piece *engine_getHeldPiece(Engine *engine) {
    return &engine->holdQueue.held;
}

Lockdown *engine_getLockdown(Engine *engine) {
    return &engine->lockdown;
}

void engine_onShiftRightDown(Engine *engine) {
    engine_shiftActive(engine, ShiftDirection_RIGHT);
}

void engine_onShiftRightUp(Engine *engine) {
    autoshift_onRelease(&engine->autoshiftVars, ShiftDirection_RIGHT);
}

void engine_onShiftLeftDown(Engine *engine) {
    engine_shiftActive(engine, ShiftDirection_LEFT);
}

void engine_onShiftLeftUp(Engine *engine) {
    autoshift_onRelease(&engine->autoshiftVars, ShiftDirection_LEFT);
}

void engine_onHardDrop(Engine *engine) {
    activePiece_slamToFloor(&engine->active);
    engine_lock(engine);
}

void engine_lock(Engine *engine) {
    activePiece_placeToField(&engine->active);
    holdQueue_onLock(&engine->holdQueue);
    engine_spawnNewPiece(engine);
}

void engine_onRotateLeft(Engine *engine) {
    engine_rotateActive(engine, ROTATION_CCW);
}

void engine_onRotateRight(Engine *engine) {
    engine_rotateActive(engine, ROTATION_CW);
}

void engine_onSoftDropDown(Engine *engine) {
    engine->gravity.softDropIsDown = true;
}

void engine_onSoftDropUp(Engine *engine) {
    engine->gravity.softDropIsDown = false;
}

void engine_onHoldDown(Engine *engine) {
    Piece holdReturn;
    if (holdQueue_performHold(&engine->holdQueue, &holdReturn, &engine->active.piece)) {
        if (holdReturn.type == PieceType_NONE) engine_spawnNewPiece(engine);
        else activePiece_newPiece(&engine->active, holdReturn);
    }

}

void engine_spawnNewPiece(Engine *engine) {
    activePiece_newPiece(&engine->active, nextQueue_next(&engine->nextQueue));
    lockdown_onPieceSpawn(&engine->lockdown, &engine->active);
}

void engine_shiftActive(Engine *engine, ShiftDirection dir) {
    if (activePiece_shift(&engine->active, (dir))) {
        lockdown_onPieceManipulate(&engine->lockdown);
    }
    autoshift_onPress(&engine->autoshiftVars, (dir));
}

void engine_rotateActive(Engine *engine, int amount) {
    if (activePiece_rotate(&engine->active, amount)) {
        lockdown_onPieceManipulate(&engine->lockdown);
    }
}
