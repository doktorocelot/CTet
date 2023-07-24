#include <malloc.h>
#include "engine.h"
#include "component/active-piece.h"
#include "../../math/prng.h"

struct Engine {
    ActivePiece active;
    Field field;
    uint64_t nextSeed;
};


Engine *engine_create() {
    Engine *engine = calloc(1, sizeof(Engine));

    activePiece_newPieceFromType(&engine->active, PieceType_T);
    engine->active.field = &engine->field;
    field_clear(&engine->field);

    return engine;
}

void engine_destroy(Engine *engine) {
    free(engine);
}

void engine_tick(Engine *engine, float deltaTime) {

}

Piece *engine_getActivePiece(Engine *engine) {
    return &engine->active.piece;
}

Point *engine_getActivePiecePos(Engine *engine) {
    return &engine->active.pos;
}

Block *engine_getFieldMatrix(Engine *engine) {
    return (Block *) engine->field.matrix;
}

void engine_onShiftRightDown(Engine *engine) {
    activePiece_shift(&engine->active, ShiftDirection_RIGHT);
}

void engine_onShiftRightUp(Engine *engine) {}

void engine_onShiftLeftDown(Engine *engine) {
    activePiece_shift(&engine->active, ShiftDirection_LEFT);
}

void engine_onShiftLeftUp(Engine *engine) {}

void engine_onHardDrop(Engine *engine) {
    activePiece_slamToFloor(&engine->active);
    activePiece_placeToField(&engine->active);
    cycleSeed(&engine->nextSeed);
    activePiece_newPieceFromType(&engine->active, engine->nextSeed % 7);
}

void engine_onRotateLeft(Engine *engine) {
    activePiece_rotate(&engine->active, ROTATION_CCW);
}

void engine_onRotateRight(Engine *engine) {
    activePiece_rotate(&engine->active, ROTATION_CW);
}
