#include "piece-data.h"

static const Point PIECE_DATA[7][4][4] = {
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

Point *pieceData_getCoords(PieceType type, Orientation orientation) {
    return (Point *) PIECE_DATA[type][orientation];
}

static const BlockColor BLOCK_COLORS[] = {
        // I, J, L, O, S, T, Z
        BlockColor_LIGHT_BLUE,
        BlockColor_BLUE,
        BlockColor_ORANGE,
        BlockColor_YELLOW,
        BlockColor_GREEN,
        BlockColor_PURPLE,
        BlockColor_RED
};

BlockColor pieceData_getColorForType(PieceType type) {
    return BLOCK_COLORS[type];
}

static const Point PIECE_SPAWN_OFFSETS[] = {
        // I, J, L, O, S, T, Z
        0, -1,
        0, 0,
        0, 0,
        1, 1,
        0, 0,
        0, 0,
        0, 0,
};

Point pieceData_getSpawnOffset(PieceType type) {
    return PIECE_SPAWN_OFFSETS[type];
}


Point pieceData_getSpawnLocation(PieceType type) {
    return point_addToNew((Point) {3, 19}, pieceData_getSpawnOffset(type));
}
