#pragma once

#include "block.h"
#include "../../../math/point.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 40
#define FIELD_NORMAL_HEIGHT 20

typedef enum {
    CoordType_EMPTY,
    CoordType_OCCUPIED,
    CoordType_OUT_OF_BOUNDS,
} CoordType;

typedef struct {
    Block matrix[FIELD_HEIGHT][FIELD_WIDTH]; // y first to make row operations easier
} Field;

void field_clear(Field *field);

CoordType field_coordTypeAt(Field *field, Point coords);

void field_setBlockAt(Field *field, Block block, Point coords);

void field_getFullRowHitList(Field *field, int *hitList);

void field_killHitList(Field *field, const int *hitList);

void field_collapseHitList(Field *field, const int *hitList);