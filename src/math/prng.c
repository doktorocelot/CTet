#include "math/math.h"

static const uint64_t a = 1664525;
static const uint64_t c = 1013904223;
static const uint64_t m = 4294967296;

void cycleSeed(uint64_t *dest) {
    const uint64_t seed = *dest;
    *dest = (a * seed + c) % m;
}
