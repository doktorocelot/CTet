#include "field.h"

static Block field_getBlockAt(Field *field, Point coords);

void field_clear(Field *field) {
    Block *matrixPtr = (Block *) field->matrix;
    for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++) {
        *matrixPtr = (Block) {.color = BlockColor_NONE};
        matrixPtr++;
    }
}

CoordType field_coordTypeAt(Field *field, Point coords) {
    if (coords.x < 0 ||
        coords.x >= FIELD_WIDTH ||
        coords.y < 0)
        return CoordType_OUT_OF_BOUNDS;

    if (coords.y >= FIELD_HEIGHT) return CoordType_EMPTY;

    if (field_getBlockAt(field, coords).color > BlockColor_NONE) return CoordType_OCCUPIED;

    return CoordType_EMPTY;
}

void field_setBlockAt(Field *field, Block block, Point coords) {
    if (coords.y >= FIELD_HEIGHT || field_coordTypeAt(field, coords) == CoordType_OUT_OF_BOUNDS) return;
    field->matrix[coords.y][coords.x] = block;
}

Block field_getBlockAt(Field *field, Point coords) {
    return field->matrix[coords.y][coords.x];
}
