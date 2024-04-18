#pragma once

#include "stdint.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/** RNG **/
void cycleSeed(uint64_t *dest);

