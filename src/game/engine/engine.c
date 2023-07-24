#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "engine.h"
#include "component/active-piece.h"
#include "../../math/prng.h"
#include "component/autoshift.h"
#include "component/gravity.h"
#include "component/next-queue.h"

struct Engine {
    Field field;
    AutoshiftVars autoshiftVars;
    Gravity gravity;
    NextQueue nextQueue;
    ActivePiece active;
};


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

    // Setup nextSeed
    engine->nextQueue = (NextQueue) {.nextSeed = time(NULL)};
    nextQueue_reset(&engine->nextQueue);

    // Setup ActivePiece
    engine_spawnNewPiece(engine);
}

void engine_tick(Engine *engine, float deltaTime) {

    int autoshiftResult;
    while (autoshiftResult = autoshift_tick(&engine->autoshiftVars, deltaTime), autoshiftResult) {
        activePiece_shift(&engine->active, autoshiftResult);
    };

    int gravityResult;
    while (gravityResult = gravity_tick(&engine->gravity, deltaTime), gravityResult) {
        if (!activePiece_dropOneLine(&engine->active)) {
            gravity_onHitFloor(&engine->gravity);
        }
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

void engine_onShiftRightDown(Engine *engine) {
    activePiece_shift(&engine->active, ShiftDirection_RIGHT);
    autoshift_onPress(&engine->autoshiftVars, ShiftDirection_RIGHT);
}

void engine_onShiftRightUp(Engine *engine) {
    autoshift_onRelease(&engine->autoshiftVars, ShiftDirection_RIGHT);
}

void engine_onShiftLeftDown(Engine *engine) {
    activePiece_shift(&engine->active, ShiftDirection_LEFT);
    autoshift_onPress(&engine->autoshiftVars, ShiftDirection_LEFT);
}

void engine_onShiftLeftUp(Engine *engine) {
    autoshift_onRelease(&engine->autoshiftVars, ShiftDirection_LEFT);
}

void engine_onHardDrop(Engine *engine) {
    activePiece_slamToFloor(&engine->active);
    activePiece_placeToField(&engine->active);
    engine_spawnNewPiece(engine);
}

void engine_onRotateLeft(Engine *engine) {
    activePiece_rotate(&engine->active, ROTATION_CCW);
}

void engine_onRotateRight(Engine *engine) {
    activePiece_rotate(&engine->active, ROTATION_CW);
}

void engine_onSoftDropDown(Engine *engine) {
    engine->gravity.softDropIsDown = true;
}

void engine_onSoftDropUp(Engine *engine) {
    engine->gravity.softDropIsDown = false;
}

void engine_spawnNewPiece(Engine *engine) {
    activePiece_newPiece(&engine->active, nextQueue_next(&engine->nextQueue));
}
