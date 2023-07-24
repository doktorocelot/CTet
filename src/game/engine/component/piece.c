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
    piece->orientation = piece_getNewOrientation(piece, amount);
    piece_updateNewOrientation(piece);
}

void piece_resetOrientation(Piece *piece) {
    piece->orientation = DEFAULT_ORIENTATION;
    piece_updateNewOrientation(piece);
}

Orientation piece_getNewOrientation(const Piece *piece, int amount) {
    return (piece->orientation + amount) % Orientation_LENGTH;
}

Point *piece_getCoordsForOrientation(Piece *piece, Orientation orientation) {
    return pieceData_getCoords(piece->type, orientation);
}

void piece_updateNewOrientation(Piece *piece) {
    piece->coords = pieceData_getCoords(piece->type, piece->orientation);
}

