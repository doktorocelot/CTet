#include <malloc.h>
#include <time.h>
#include <ctet/ctet.h>
#include "../math/prng.h"

#include "component/active-piece.h"
#include "behavior/lockdown.h"
#include "component/hold-queue.h"
#include "component/next-queue.h"
#include "behavior/gravity.h"
#include "behavior/autoshift.h"

#include <math.h>

struct CTetEngine {
    Field field;
    AutoshiftVars autoshiftVars;
    Gravity gravity;
    NextQueue nextQueue;
    HoldQueue holdQueue;
    Lockdown lockdown;
    ActivePiece active;
    double timeElapsed;
    CTetMessage messages[256];
    int msgInsertPtr;
    int msgPullPtr;
    CTetStats stats;
};

static int retryCount = 0;

static void pushMessage(CTetEngine *engine, CTetMessageId id, int32_t detailA, int32_t detailB);

static void spawnNextPiece(CTetEngine *engine, CTetPiece piece);

static void lockdown(CTetEngine *engine);

static void shiftActive(CTetEngine *engine, ShiftDirection dir);

static void rotateActive(CTetEngine *engine, int amount);

static void clearEngineMsgPtrs(CTetEngine *engine) {
    engine->msgInsertPtr = 0;
    engine->msgPullPtr = 0;
}

void ctEngine_init(CTetEngine *engine) {
    engine->active.field = &engine->field;
    ctEngine_reset(engine);
    clearEngineMsgPtrs(engine);
}

void ctEngine_reset(CTetEngine *engine) {
    // Setup Field
    field_clear(&engine->field);

    // Setup Autoshift
    engine->autoshiftVars = (AutoshiftVars) {0};

    // Setup Gravity
    engine->gravity = (Gravity) {.msPerRow = 1000};

    // Setup NEXT
    engine->nextQueue = (NextQueue) {.nextSeed = time(NULL) + retryCount * (retryCount % 10)};
    retryCount++;
    nextQueue_reset(&engine->nextQueue);

    // Setup HOLD
    holdQueue_reset(&engine->holdQueue);

    // Setup Lockdown
    engine->lockdown = (Lockdown) {500};

    // Setup ActivePiece
    spawnNextPiece(engine, nextQueue_next(&engine->nextQueue));

    // Reset Time Elapsed
    engine->timeElapsed = 0;

    // Reset Stats
    engine->stats = (CTetStats) {.level = 1};
}

void ctEngine_update(CTetEngine *engine, const float deltaMillis) {
    int autoshiftResult;
    while (autoshiftResult = autoshift_update(&engine->autoshiftVars, deltaMillis), autoshiftResult) {
        shiftActive(engine, autoshiftResult);
    }

    int gravityResult;
    while (gravityResult = gravity_update(&engine->gravity, &engine->active, deltaMillis), gravityResult) {
        if (engine->gravity.softDropIsDown) engine->stats.score++;
        if (!activePiece_dropOneLine(&engine->active)) {
            gravity_onHitFloor(&engine->gravity);
        }
    }

    if (lockdown_update(&engine->lockdown, &engine->active, deltaMillis)) {
        lockdown(engine);
    }
    engine->timeElapsed += deltaMillis;
}

static int scoreLineLut[] = {
    0, 100, 300, 500, 800
};

void lockdown(CTetEngine *engine) {
    activePiece_placeToField(&engine->active, engine->timeElapsed);
    pushMessage(engine, CT_MSG_LOCKDOWN, 0, 0);
    holdQueue_onLock(&engine->holdQueue);

    engine->stats.pieces++;

    int hitList[CT_TOTAL_FIELD_HEIGHT + 1];
    int lines = 0;
    field_getFullRowHitList(&engine->field, hitList, &lines);
    field_killHitList(&engine->field, hitList);
    field_collapseHitList(&engine->field, hitList);

    engine->stats.lines += lines;
    engine->stats.level = engine->stats.lines / 10 + 1;
    engine->stats.score += scoreLineLut[lines] * engine->stats.level;

    const float x = engine->stats.level;
    float fallSpeed = pow(0.8f - (x - 1.0f) * 0.007f, x - 1.0f);
    fallSpeed *= 1000;
    if (engine->stats.level > 20) {
        const float i = x - 20;
        float lockDelay = 500 - 10 * i;
        engine->lockdown.lockDelay = lockDelay;
    }
    engine->gravity.msPerRow = fallSpeed;

    spawnNextPiece(engine, nextQueue_next(&engine->nextQueue));
    if (engine->stats.level >= 30) {
        for (int i = 0; i < CT_BLOCKS_PER_PIECE; i++) {
            engine->nextQueue.pieces[4].blocks[i].color = CTetBlockColor_BONE;
        }
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
    int distance;
    activePiece_slamToFloor(&engine->active, &distance);
    lockdown(engine);
    engine->stats.score += distance * 2;
    pushMessage(engine, CT_MSG_HARD_DROP, 0, 0);
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

CTetPoint ctEngine_getActivePiecePos(const CTetEngine *engine) {
    return engine->active.pos;
}

CTetPoint ctEngine_getGhostOffset(const CTetEngine *engine) {
    return (CTetPoint) {0, -activePiece_getDistanceToGround(&engine->active)};
}

const CTetPiece *ctEngine_getActivePiece(const CTetEngine *engine) {
    return &engine->active.piece;
}

float ctEngine_getLockDelayRemainingPercentage(const CTetEngine *engine) {
    return lockdown_lockDelayRemaining(&engine->lockdown);
}

const CTetPiece *ctEngine_getNextPieces(const CTetEngine *engine) {
    return engine->nextQueue.pieces;
}

const CTetPiece *ctEngine_getHeldPiece(const CTetEngine *engine) {
    return &engine->holdQueue.held;
}

bool ctEngine_holdIsLocked(const CTetEngine *engine) {
    return holdQueue_isLocked(&engine->holdQueue);
}

const CTetBlock *ctEngine_getBlockAtFieldLocation(const CTetEngine *engine, const CTetPoint location) {
    return &engine->field.matrix[location.y][location.x];
}

double ctEngine_getTimestamp(const CTetEngine *engine) {
    return engine->timeElapsed;
}

CTetMessage ctEngine_nextMessage(CTetEngine *engine) {
    if (engine->msgInsertPtr == 0) {
        return (CTetMessage) {.id = CT_MSG_NONE};
    }
    if (engine->msgInsertPtr == engine->msgPullPtr) {
        clearEngineMsgPtrs(engine);
        return (CTetMessage) {.id = CT_MSG_NONE};
    }
    return engine->messages[engine->msgPullPtr++];
}

size_t ctEngine_getSize() {
    return sizeof(CTetEngine);
}

const CTetStats *ctEngine_getStats(const CTetEngine *engine) {
    return &engine->stats;
}

void pushMessage(CTetEngine *engine, const CTetMessageId id, const int32_t detailA, const int32_t detailB) {
    engine->messages[engine->msgInsertPtr++] = (CTetMessage) {.id = id, .detailA = detailA, .detailB = detailB};
}

void spawnNextPiece(CTetEngine *engine, const CTetPiece piece) {
    activePiece_newPiece(&engine->active, piece);
    if (activePiece_collidesWith(&engine->active, (CTetPoint) {0})) {
        pushMessage(engine, CT_MSG_GAME_OVER, CT_GAME_OVER_TYPE_BLOCK_OUT, 0);
    }
    lockdown_onPieceSpawn(&engine->lockdown, &engine->active);
}

void shiftActive(CTetEngine *engine, const ShiftDirection dir) {
    if (activePiece_shift(&engine->active, dir)) {
        pushMessage(engine, CT_MSG_SHIFT, 0, 0);
        lockdown_onPieceManipulate(&engine->lockdown);
    }
}

void rotateActive(CTetEngine *engine, const int amount) {
    if (activePiece_rotate(&engine->active, amount)) {
        lockdown_onPieceManipulate(&engine->lockdown);
    }
}
