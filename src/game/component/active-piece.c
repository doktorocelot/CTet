#include "game/component/component.h"
#include "game/data/data.h"
#include "game/behavior/behavior.h"

void activePiece_newPiece(ActivePiece *active, const CTetPiece piece) {
    active->piece = piece;
    active->pos = pieceData_getSpawnLocation(piece.type);
    activePiece_dropOneLine(active);
}

bool activePiece_shift(ActivePiece *active, const ShiftDirection direction) {
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

bool activePiece_collidesWithOrientation(const ActivePiece *active, const CTetOrientation orientation, const CTetPoint offset) {
    const CTetPoint *coordPtr = piece_getCoordsForOrientation(&active->piece, orientation);
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

bool activePiece_collidesWith(const ActivePiece *active, const CTetPoint offset) {
    return activePiece_collidesWithOrientation(active, active->piece.orientation, offset);
}

void activePiece_slamToFloor(ActivePiece *active, int *distanceOut) {
    const int distance = activePiece_getDistanceToGround(active);
    active->pos.y -= distance;
    if (distanceOut != NULL) *distanceOut = distance; 
}

int activePiece_getDistanceToGround(const ActivePiece *active) {
    int distance = 0;
    while (!activePiece_collidesWith(active, (CTetPoint) {0, -distance - 1})) {
        distance++;
    }
    return distance;
}

void activePiece_placeToField(ActivePiece *active, const double lockTimestamp) {
    for (int i = 0; i < CT_BLOCKS_PER_PIECE; i++) {
        active->piece.blocks[i].lockedTimestamp = lockTimestamp;
        field_setBlockAt(active->field,
                         active->piece.blocks[i],
                         ctPoint_addToNew(active->piece.coords[i], active->pos));
    }
}

bool activePiece_rotate(ActivePiece *active, const int amount) {
    CTetPoint wallkickResult;
    if (executeWallkick(&wallkickResult, active, amount)) {
        piece_rotate(&active->piece, amount);
        ctPoint_add(&active->pos, wallkickResult);
        return true;
    }
    return false;
}



