#include "piece.h"
#include "../data/piece-data.h"

#define DEFAULT_ORIENTATION Orientation_UP

Piece piece_buildFromType(PieceType type) {
    Piece piece = {
            .coords = pieceData_getCoords(type, DEFAULT_ORIENTATION),
            .orientation = DEFAULT_ORIENTATION,
            .type = type,
    };

    BlockColor color = pieceData_getColorForType(type);
    for (int i = 0; i < BLOCKS_PER_PIECE; i++) {
        piece.blocks[i] = (Block) {.color = color};
    }

    return piece;
}

static void piece_updateNewOrientation(Piece *piece);

void piece_rotate(Piece *piece, int amount) {
    piece->orientation = (piece->orientation + amount) % Orientation_LENGTH;
    piece_updateNewOrientation(piece);
}

void piece_updateNewOrientation(Piece *piece) {
    piece->coords = pieceData_getCoords(piece->type, piece->orientation);
}

