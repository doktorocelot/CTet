#include "active-piece.h"

void activePiece_newPieceFromType(ActivePiece *active, PieceType type) {
    active->piece = piece_buildFromType(type);
    active->pos = (Point) {4, 18};
}

void activePiece_shift(ActivePiece *active, ShiftDirection direction) {
    if (!activePiece_collidesWith(active, (Point) {direction, 0})) {
        active->pos.x += direction;
    }
}

bool activePiece_collidesWith(ActivePiece *active, Point offset) {
    Point *coordPtr = active->piece.coords;
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

void activePiece_slamToFloor(ActivePiece *active) {
    int i = 0;
    while (!activePiece_collidesWith(active, (Point) {0, -1}) && i < 30) {
        active->pos.y--;
        i++;
    }
}

void activePiece_placeToField(ActivePiece *active) {
    for (int i = 0; i < BLOCKS_PER_PIECE; i++) {
        Piece piece = active->piece;
        field_setBlockAt(active->field,
                         piece.blocks[i],
                         point_addToNew(piece.coords[i], active->pos));
    }
}

