#include "active-piece.h"
#include "../data/piece-data.h"
#include "wallkick.h"

bool activePiece_newPiece(ActivePiece *active, CTetPiece piece) {
    active->piece = piece;
    active->pos = pieceData_getSpawnLocation(piece.type);
    activePiece_dropOneLine(active);
    if (activePiece_collidesWith(active, (CTetPoint) {0})) return false;
    return true;
}

bool activePiece_shift(ActivePiece *active, ShiftDirection direction) {
    if (!activePiece_collidesWith(active, (CTetPoint) {direction, 0})) {
        active->pos.x += direction;
        return true;
    }
    return false;
}

bool activePiece_dropOneLine(ActivePiece *active) {
    if (activePiece_collidesWith(active, (CTetPoint) {0, -1})) return false;
    active->pos.y--;
    return true;
}

bool activePiece_collidesWithOrientation(ActivePiece *active, CTetOrientation orientation, CTetPoint offset) {
    CTetPoint *coordPtr = piece_getCoordsForOrientation(&active->piece, orientation);
    for (int i = 0; i < CT_BLOCKS_PER_PIECE; i++) {
        if (field_coordTypeAt(active->field,
                              ctPoint_addToNew(ctPoint_addToNew(active->pos, offset), *coordPtr)
        ) != CoordType_EMPTY) {
            return true;
        }
        coordPtr++;
    }
    return false;
}

bool activePiece_collidesWith(ActivePiece *active, CTetPoint offset) {
    return activePiece_collidesWithOrientation(active, active->piece.orientation, offset);
}

void activePiece_slamToFloor(ActivePiece *active) {
    active->pos.y -= activePiece_getDistanceToGround(active);
}

int activePiece_getDistanceToGround(ActivePiece *active) {
    int distance = 0;
    while (!activePiece_collidesWith(active, (CTetPoint) {0, -distance - 1})) {
        distance++;
    }
    return distance;
}

void activePiece_placeToField(ActivePiece *active) {
    for (int i = 0; i < CT_BLOCKS_PER_PIECE; i++) {
        CTetPiece piece = active->piece;
        field_setBlockAt(active->field,
                         piece.blocks[i],
                         ctPoint_addToNew(piece.coords[i], active->pos));
    }
}

bool activePiece_rotate(ActivePiece *active, int amount) {
    CTetPoint wallkickResult;
    if (executeWallkick(&wallkickResult, active, amount)) {
        piece_rotate(&active->piece, amount);
        ctPoint_add(&active->pos, wallkickResult);
        return true;
    }
    return false;
}



