#include <memory.h>
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

void field_getFullRowHitList(Field *field, int *hitList) {
    int hitListIndex = 0;
    for (int y = FIELD_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x <= FIELD_WIDTH; x++) {
            if (x == FIELD_WIDTH) {
                hitList[hitListIndex] = y;
                hitListIndex++;
                break;
            }
            if (field_getBlockAt(field, (Point) {x, y}).color <= BlockColor_NONE) break;
        }
    }
    hitList[hitListIndex] = HIT_LIST_END;
}

static void field_killRow(Field *field, int row);

void field_killHitList(Field *field, const int *hitList) {
    for (int i = 0; hitList[i] != HIT_LIST_END && i <= FIELD_HEIGHT; i++) {
        field_killRow(field, hitList[i]);
    }
}

void field_collapseHitList(Field *field, const int *hitList) {
    int rowToKill;
    for (int i = 0; rowToKill = hitList[i], rowToKill != HIT_LIST_END && i <= FIELD_HEIGHT; i++) {
        memmove(
                field->matrix[rowToKill],
                field->matrix[rowToKill + 1],
                FIELD_WIDTH * (FIELD_HEIGHT - rowToKill) * sizeof(Block)
        );
        // Don't forget to clear the top row, so we don't get duplicates!
        // ...but of course, nobody would forget such a basic thing, right?
        field_killRow(field, FIELD_HEIGHT - 1);
    }
}

void field_killRow(Field *field, int row) {
    memset(field->matrix[row], 0, FIELD_WIDTH * sizeof(Block));
}