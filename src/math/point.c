#include "point.h"

Point point_addToNew(Point a, Point b) {
    return (Point) {a.x + b.x, a.y + b.y};
}

