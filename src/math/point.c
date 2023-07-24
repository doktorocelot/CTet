#include "point.h"

Point point_addToNew(Point a, Point b) {
    return (Point) {a.x + b.x, a.y + b.y};
}

void point_add(Point *dest, Point addend) {
    dest->x += addend.x;
    dest->y += addend.y;
}

