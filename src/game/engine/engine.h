#pragma once

#include "component/piece.h"
#include "component/lockdown.h"
#include "component/hold-queue.h"
#include "component/next-queue.h"
#include "component/gravity.h"
#include "component/autoshift.h"

typedef struct {
    Field field;
    AutoshiftVars autoshiftVars;
    Gravity gravity;
    NextQueue nextQueue;
    HoldQueue holdQueue;
    Lockdown lockdown;
    ActivePiece active;
    bool isDead;
} Engine;

Engine *engine_create();

void engine_destroy(Engine *engine);

void engine_reset(Engine *engine);

void engine_tick(Engine *engine, float deltaTime);

void engine_onShiftRightDown(Engine *engine);

void engine_onShiftRightUp(Engine *engine);

void engine_onShiftLeftDown(Engine *engine);

void engine_onShiftLeftUp(Engine *engine);

void engine_onHardDrop(Engine *engine);

void engine_onRotateLeft(Engine *engine);

void engine_onRotateRight(Engine *engine);

void engine_onSoftDropDown(Engine *engine);

void engine_onSoftDropUp(Engine *engine);

void engine_onHoldDown(Engine *engine);

void engine_spawnNewPiece(Engine *engine, Piece piece);

bool engine_placingPieceWillDie(Engine *engine);