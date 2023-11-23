#pragma once

#include <ctet/ctet.h>

#define HIT_LIST_END (-1)
typedef enum {
    CoordType_EMPTY,
    CoordType_OCCUPIED,
    CoordType_OUT_OF_BOUNDS,
} CoordType;

typedef struct {
    CTetBlock matrix[CT_TOTAL_FIELD_HEIGHT][CT_FIELD_WIDTH]; // y first to make row operations easier
} Field;

void field_clear(Field *field);

CoordType field_coordTypeAt(const Field *field, CTetPoint coords);

void field_setBlockAt(Field *field, CTetBlock block, CTetPoint coords);

void field_getFullRowHitList(const Field *field, int *hitList);

void field_killHitList(Field *field, const int *hitList);

void field_collapseHitList(Field *field, const int *hitList);