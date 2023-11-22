#include "wallkick.h"
#include "piece.h"

// order:
// starting orientation;
// rotate right, rotate left
// index;
static const CTetPoint WALLKICK_DATA_X[4][2][5] = {
        { // up
                0, 0, -1, 0, -1, 1,  0, -2, -1, -2,
                0, 0, 1,  0, 1,  1,  0, -2, 1,  -2,
        },
        { // right
                0, 0, 1,  0, 1,  -1, 0, 2,  1,  2,
                0, 0, 1,  0, 1,  -1, 0, 2,  1,  2
        },
        { // down
                0, 0, 1,  0, 1,  1,  0, -2, 1,  -2,
                0, 0, -1, 0, -1, 1,  0, -2, -1, -2,
        },
        { // left
                0, 0, -1, 0, -1, -1, 0, 2,  -1, 2,
                0, 0, -1, 0, -1, -1, 0, 2,  -1, 2,
        }
};
static const CTetPoint WALLKICK_DATA_I[4][2][5] = {
        { // up
                0, 0, -2, 0, 1,  0, -2, -1, 1,  2,
                0, 0, -1, 0, 2,  0, -1, 2,  2,  -1,
        },
        { // right
                0, 0, -1, 0, 2,  0, -1, 2,  2,  -1,
                0, 0, 2,  0, -1, 0, 2,  1,  -1, -2,
        },
        { // down
                0, 0, 2,  0, -1, 0, 2,  1,  -1, -2,
                0, 0, 1,  0, -2, 0, 1,  -2, -2, 1,
        },
        { // left
                0, 0, 1,  0, -2, 0, 1,  -2, -2, 1,
                0, 0, -2, 0, 1,  0, -2, -1, 1,  2,
        }
};

// No kicks
static const CTetPoint WALLKICK_DATA_NONE[5] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static const CTetPoint *getWallkickData(const CTetPiece *piece, int direction);

bool executeWallkick(CTetPoint *dest, const ActivePiece *active, const int direction) {
        const CTetPoint *wallkickData = getWallkickData(&active->piece, direction);
    for (int i = 0; i < 5; i++) {
        if (!activePiece_collidesWithOrientation(
                active,
                piece_getNewOrientation(&active->piece, direction),
                *wallkickData
        )) {
            *dest = *wallkickData;
            return true;
        }
        wallkickData++;
    }
    return false;
}

const CTetPoint *getWallkickData(const CTetPiece *piece, const int direction) {
        const int directionIndex = direction == CT_ROTATION_CCW ? 1 : 0;
    switch (piece->type) {
        case CTetPieceType_O:
            return WALLKICK_DATA_NONE;
        case CTetPieceType_I:
            return WALLKICK_DATA_I[piece->orientation][directionIndex];
        default:
            return WALLKICK_DATA_X[piece->orientation][directionIndex];
    }
}
