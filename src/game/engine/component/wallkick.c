#include "wallkick.h"

// order:
// starting orientation;
// rotate right, rotate left
// index;
static const Point WALLKICK_DATA_X[4][2][5] = {
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
static const Point WALLKICK_DATA_I[4][2][5] = {
        { // up
                0, 0, -2, 0, 1,  0, -2, -1, 1,  2,
                0, 0, -1, 0, 2,  0, -1, 2,  2,  -1,
        },
        { // right
                0, 0, -1, 0, 2,  0, -1, 2,  2,  -1,
                0, 0, 2,  0, -1, 0, 2,  1,  -1, -2,
        },
        { // down
                0, 0, 2,  0, -1, 0, 2,  1,  -1, 2,
                0, 0, 1,  0, -2, 0, 1,  -2, -2, 1,
        },
        { // left
                0, 0, 1,  0, -2, 0, 1,  -2, -2, 1,
                0, 0, -2, 0, 1,  0, -2, -1, 1,  2,
        }
};

// No kicks
static const Point WALLKICK_DATA_NONE[5] = {
        0,0,0,0,0,0,0,0,0,0,
};

static Point *getWallkickData(Piece *piece, int direction);

bool executeWallkick(Point *dest, ActivePiece *active, int direction) {
    Point *wallkickData = getWallkickData(&active->piece, direction);
    for (int i = 0; i < 5; i++) {
        if (!activePiece_collidesWithOrientation(active,
                                                 piece_getNewOrientation(&active->piece, direction),
                                                 *wallkickData)) {
            *dest = *wallkickData;
            return true;
        }
        wallkickData++;
    }
    return false;
}

Point *getWallkickData(Piece *piece, int direction) {
    int directionIndex = direction == ROTATION_CCW ? 1 : 0;
    switch (piece->type) {
        case PieceType_O:
            return (Point *) WALLKICK_DATA_NONE;
        case PieceType_I:
            return (Point *) WALLKICK_DATA_I[piece->orientation][directionIndex];
        default:
            return (Point *) WALLKICK_DATA_X[piece->orientation][directionIndex];
    }
}
