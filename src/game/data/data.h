#pragma once

#include <ctet/ctet.h>

/** Piece Data **/

// Returns a pointer to the first coords (of four) of a certain piece type and orientation
const CTetPoint* pieceData_getCoords(CTetPieceType type, CTetOrientation orientation);

CTetBlockColor pieceData_getColorForType(CTetPieceType type);

CTetPoint pieceData_getSpawnOffset(CTetPieceType type);

CTetPoint pieceData_getSpawnLocation(CTetPieceType type);