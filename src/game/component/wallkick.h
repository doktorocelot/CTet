#pragma once

#include <stdbool.h>
#include "active-piece.h"

bool executeWallkick(CTetPoint *dest, const ActivePiece *active, int direction);