#pragma once

#include "component/piece.h"

typedef struct Engine Engine;

Engine *engine_create();

void engine_destroy(Engine *engine);

void engine_tick(Engine *engine, float deltaTime);

Piece *engine_getActivePiece(Engine *engine);

Point *engine_getActivePiecePos(Engine *engine);

Block *engine_getFieldMatrix(Engine *engine);

void engine_onShiftRightDown(Engine *engine);

void engine_onShiftRightUp(Engine *engine);

void engine_onShiftLeftDown(Engine *engine);

void engine_onShiftLeftUp(Engine *engine);

void engine_onHardDrop(Engine *engine);

void engine_onRotateLeft(Engine *engine);

void engine_onRotateRight(Engine *engine);