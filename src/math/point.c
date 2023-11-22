#include <ctet/ctet.h>

CTetPoint ctPoint_addToNew(const CTetPoint a, const CTetPoint b) {
    return (CTetPoint) {a.x + b.x, a.y + b.y};
}

void ctPoint_add(CTetPoint *dest, const CTetPoint addend) {
    dest->x += addend.x;
    dest->y += addend.y;
}

bool ctPoint_isEqual(const CTetPoint a, const CTetPoint b) {
    return a.x == b.x && a.y == b.y;
}

