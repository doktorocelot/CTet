#include "piece.h"
#include "../data/piece-data.h"

#define DEFAULT_ORIENTATION CTetOrientation_UP

CTetPiece piece_buildFromType(const CTetPieceType type) {
    CTetPiece piece = {
            .coords = pieceData_getCoords(type, DEFAULT_ORIENTATION),
            .orientation = DEFAULT_ORIENTATION,
            .type = type,
    };

    const CTetBlockColor color = pieceData_getColorForType(type);
    for (int i = 0; i < CT_BLOCKS_PER_PIECE; i++) {
        piece.blocks[i] = (CTetBlock) {
            .color = color,
            .lockedTimestamp = BLOCK_TIMESTAMP_NOT_LOCKED,
        };
    }

    return piece;
}

static void piece_updateNewOrientation(CTetPiece *piece);



void piece_rotate(CTetPiece *piece, const int amount) {
    piece->orientation = piece_getNewOrientation(piece, amount);
    piece_updateNewOrientation(piece);
}

void piece_resetOrientation(CTetPiece *piece) {
    piece->orientation = DEFAULT_ORIENTATION;
    piece_updateNewOrientation(piece);
}

CTetOrientation piece_getNewOrientation(const CTetPiece *piece, const int amount) {
    return (piece->orientation + amount) % Orientation_LENGTH;
}

const CTetPoint* piece_getCoordsForOrientation(const CTetPiece *piece, const CTetOrientation orientation) {
    return pieceData_getCoords(piece->type, orientation);
}

void piece_updateNewOrientation(CTetPiece *piece) {
    piece->coords = pieceData_getCoords(piece->type, piece->orientation);
}

