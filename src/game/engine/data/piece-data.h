#pragma once

#include "../component/piece.h"
#include "../../../math/point.h"

// Returns a pointer to the first coords (of four) of a certain piece type and orientation
Point *pieceData_getCoords(PieceType type, Orientation orientation);

BlockColor pieceData_getColorForType(PieceType type);

Point pieceData_getSpawnOffset(PieceType type);