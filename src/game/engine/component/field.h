#pragma once

#include "block.h"
#include "../../../math/point.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

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