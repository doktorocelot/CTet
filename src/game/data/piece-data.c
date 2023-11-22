#include "piece-data.h"

#include "ctet/ctet.h"

static const CTetPoint PIECE_DATA[7][4][4] = {
        // I
        0, 2, 1, 2, 2, 2, 3, 2,
        2, 3, 2, 2, 2, 1, 2, 0,
        3, 1, 2, 1, 1, 1, 0, 1,
        1, 0, 1, 1, 1, 2, 1, 3,
        // J
        0, 2, 0, 1, 1, 1, 2, 1,
        2, 2, 1, 2, 1, 1, 1, 0,
        2, 0, 2, 1, 1, 1, 0, 1,
        0, 0, 1, 0, 1, 1, 1, 2,
        // L
        2, 2, 2, 1, 1, 1, 0, 1,
        2, 0, 1, 0, 1, 1, 1, 2,
        0, 0, 0, 1, 1, 1, 2, 1,
        0, 2, 1, 2, 1, 1, 1, 0,
        // O
        0, 0, 0, 1, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 0, 0, 0,
        1, 1, 1, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 1, 1, 1,
        // S
        2, 2, 1, 2, 1, 1, 0, 1,
        2, 0, 2, 1, 1, 1, 1, 2,
        0, 0, 1, 0, 1, 1, 2, 1,
        0, 2, 0, 1, 1, 1, 1, 0,
        // T
        1, 2, 0, 1, 1, 1, 2, 1,
        2, 1, 1, 2, 1, 1, 1, 0,
        1, 0, 2, 1, 1, 1, 0, 1,
        0, 1, 1, 0, 1, 1, 1, 2,
        // Z
        0, 2, 1, 2, 1, 1, 2, 1,
        2, 2, 2, 1, 1, 1, 1, 0,
        2, 0, 1, 0, 1, 1, 0, 1,
        0, 0, 0, 1, 1, 1, 1, 2,
};

const CTetPoint* pieceData_getCoords(const CTetPieceType type, const CTetOrientation orientation) {
    return PIECE_DATA[type][orientation];
}

static const CTetBlockColor BLOCK_COLORS[] = {
        // I, J, L, O, S, T, Z
        CTetBlockColor_LIGHT_BLUE,
        CTetBlockColor_BLUE,
        CTetBlockColor_ORANGE,
        CTetBlockColor_YELLOW,
        CTetBlockColor_GREEN,
        CTetBlockColor_PURPLE,
        CTetBlockColor_RED
};

CTetBlockColor pieceData_getColorForType(const CTetPieceType type) {
    return BLOCK_COLORS[type];
}

static const CTetPoint PIECE_SPAWN_OFFSETS[] = {
        // I, J, L, O, S, T, Z
        0, -1,
        0, 0,
        0, 0,
        1, 1,
        0, 0,
        0, 0,
        0, 0,
};

CTetPoint pieceData_getSpawnOffset(const CTetPieceType type) {
    return PIECE_SPAWN_OFFSETS[type];
}


CTetPoint pieceData_getSpawnLocation(const CTetPieceType type) {
    return ctPoint_addToNew((CTetPoint) {3, 19}, pieceData_getSpawnOffset(type));
}
