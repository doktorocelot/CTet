#pragma once

#include <stdbool.h>

typedef struct {
    int x, y;
} Point;

Point point_addToNew(Point a, Point b);

void point_add(Point *dest, Point addend);

bool point_isEqual(Point a, Point b);