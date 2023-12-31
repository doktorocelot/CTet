#pragma once

#include <ctet/ctet.h>

#define CT_ROTATION_CW 1
#define CT_ROTATION_CCW 3
#define CT_ROTATION_180 2

CTetPiece piece_buildFromType(CTetPieceType type);

void piece_rotate(CTetPiece *piece, int amount);

void piece_resetOrientation(CTetPiece *piece);

CTetOrientation piece_getNewOrientation(const CTetPiece *piece, int amount);

const CTetPoint* piece_getCoordsForOrientation(const CTetPiece *piece, CTetOrientation orientation);