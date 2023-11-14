#include <ctet/ctet.h>

CTetPoint ctPoint_addToNew(CTetPoint a, CTetPoint b) {
    return (CTetPoint) {a.x + b.x, a.y + b.y};
}

void ctPoint_add(CTetPoint *dest, CTetPoint addend) {
    dest->x += addend.x;
    dest->y += addend.y;
}

bool ctPoint_isEqual(CTetPoint a, CTetPoint b) {
    return a.x == b.x && a.y == b.y;
}

