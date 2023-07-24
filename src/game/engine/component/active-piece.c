#include "active-piece.h"
#include "../data/piece-data.h"

void activePiece_newPiece(ActivePiece *active, Piece piece) {
    active->piece = piece;
    active->pos = point_addToNew((Point) {3, 19}, pieceData_getSpawnOffset(piece.type));
    activePiece_dropOneLine(active);
}

void activePiece_shift(ActivePiece *active, ShiftDirection direction) {
    if (!activePiece_collidesWith(active, (Point) {direction, 0})) {
        active->pos.x += direction;
    }
}

bool activePiece_dropOneLine(ActivePiece *active) {
    if (activePiece_collidesWith(active, (Point) {0, -1})) return false;
    active->pos.y--;
    return true;
}

bool activePiece_collidesWithOrientation(ActivePiece *active, Orientation orientation, Point offset) {
    Point *coordPtr = piece_getCoordsForOrientation(&active->piece, orientation);
    for (int i = 0; i < BLOCKS_PER_PIECE; i++) {
        if (field_coordTypeAt(active->field,
                              point_addToNew(point_addToNew(active->pos, offset), *coordPtr)
        ) != CoordType_EMPTY) {
            return true;
        }
        coordPtr++;
    }
    return false;
}

bool activePiece_collidesWith(ActivePiece *active, Point offset) {
    return activePiece_collidesWithOrientation(active, active->piece.orientation, offset);
}

void activePiece_slamToFloor(ActivePiece *active) {
    active->pos.y -= activePiece_getDistanceToGround(active);
}

int activePiece_getDistanceToGround(ActivePiece *active) {
    int distance = 0;
    while (!activePiece_collidesWith(active, (Point) {0, -distance - 1})) {
        distance++;
    }
    return distance;
}

void activePiece_placeToField(ActivePiece *active) {
    for (int i = 0; i < BLOCKS_PER_PIECE; i++) {
        Piece piece = active->piece;
        field_setBlockAt(active->field,
                         piece.blocks[i],
                         point_addToNew(piece.coords[i], active->pos));
    }
}

void activePiece_rotate(ActivePiece *active, int amount) {
    if (!activePiece_collidesWithOrientation(
            active,
            piece_getNewOrientation(&active->piece, amount),
            (Point) {0}
    )) {
        piece_rotate(&active->piece, amount);
    }
}



