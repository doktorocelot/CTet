#pragma once

typedef struct {
    int x, y;
} Point;

Point point_addToNew(Point a, Point b);

void point_add(Point *dest, Point addend);