#include <memory.h>
#include "field.h"

static CTetBlock field_getBlockAt(const Field *field, CTetPoint coords);

void field_clear(const Field *field) {
    CTetBlock *matrixPtr = field->matrix;
    for (int i = 0; i < CT_FIELD_WIDTH * CT_TOTAL_FIELD_HEIGHT; i++) {
        *matrixPtr = (CTetBlock) {.color = CTetBlockColor_NONE};
        matrixPtr++;
    }
}

CoordType field_coordTypeAt(const Field *field, const CTetPoint coords) {
    if (coords.x < 0 ||
        coords.x >= CT_FIELD_WIDTH ||
        coords.y < 0)
        return CoordType_OUT_OF_BOUNDS;

    if (coords.y >= CT_TOTAL_FIELD_HEIGHT) return CoordType_EMPTY;

    if (field_getBlockAt(field, coords).color > CTetBlockColor_NONE) return CoordType_OCCUPIED;

    return CoordType_EMPTY;
}

void field_setBlockAt(Field *field, const CTetBlock block, const CTetPoint coords) {
    if (coords.y >= CT_TOTAL_FIELD_HEIGHT || field_coordTypeAt(field, coords) == CoordType_OUT_OF_BOUNDS) return;
    field->matrix[coords.y][coords.x] = block;
}


CTetBlock field_getBlockAt(const Field *field, const CTetPoint coords) {
    return field->matrix[coords.y][coords.x];
}

void field_getFullRowHitList(const Field *field, int *hitList) {
    int hitListIndex = 0;
    for (int y = CT_TOTAL_FIELD_HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x <= CT_FIELD_WIDTH; x++) {
            if (x == CT_FIELD_WIDTH) {
                hitList[hitListIndex] = y;
                hitListIndex++;
                break;
            }
            if (field_getBlockAt(field, (CTetPoint) {x, y}).color <= CTetBlockColor_NONE) break;
        }
    }
    hitList[hitListIndex] = HIT_LIST_END;
}

static void field_killRow(Field *field, int row);

void field_killHitList(Field *field, const int *hitList) {
    for (int i = 0; hitList[i] != HIT_LIST_END && i <= CT_TOTAL_FIELD_HEIGHT; i++) {
        field_killRow(field, hitList[i]);
    }
}

void field_collapseHitList(Field *field, const int *hitList) {
    int rowToKill;
    for (int i = 0; rowToKill = hitList[i], rowToKill != HIT_LIST_END && i <= CT_TOTAL_FIELD_HEIGHT; i++) {
        memmove(
                field->matrix[rowToKill],
                field->matrix[rowToKill + 1],
                CT_FIELD_WIDTH * (CT_TOTAL_FIELD_HEIGHT - rowToKill) * sizeof(CTetBlock)
        );
        // Don't forget to clear the top row, so we don't get duplicates!
        // ...but of course, nobody would forget such a basic thing, right?
        field_killRow(field, CT_TOTAL_FIELD_HEIGHT - 1);
    }
}

void field_killRow(Field *field, const int row) {
    memset(field->matrix[row], 0, CT_FIELD_WIDTH * sizeof(CTetBlock));
}