#pragma once

#include <ctet/ctet.h>
#include <stdbool.h>
#include <stdint.h>

/** Piece **/

#define CT_ROTATION_CW 1
#define CT_ROTATION_CCW 3
#define CT_ROTATION_180 2

CTetPiece piece_buildFromType(CTetPieceType type);

void piece_rotate(CTetPiece *piece, int amount);

void piece_resetOrientation(CTetPiece *piece);

CTetOrientation piece_getNewOrientation(const CTetPiece *piece, int amount);

const CTetPoint* piece_getCoordsForOrientation(const CTetPiece *piece, CTetOrientation orientation);


/** Field **/

#define HIT_LIST_END (-1)
typedef enum {
    CoordType_EMPTY,
    CoordType_OCCUPIED,
    CoordType_OUT_OF_BOUNDS,
} CoordType;

typedef struct {
    CTetBlock matrix[CT_TOTAL_FIELD_HEIGHT][CT_FIELD_WIDTH]; // y first to make row operations easier
} Field;

void field_clear(Field *field);

CoordType field_coordTypeAt(const Field *field, CTetPoint coords);

void field_setBlockAt(Field *field, CTetBlock block, CTetPoint coords);

void field_getFullRowHitList(const Field *field, int *hitList, int *totalLines);

void field_killHitList(Field *field, const int *hitList);

void field_collapseHitList(Field *field, const int *hitList);


/** Active Piece **/

typedef struct {
    CTetPoint pos;
    CTetPiece piece;
    Field *field;
} ActivePiece;

typedef enum {
    ShiftDirection_LEFT = -1,
    ShiftDirection_NONE,
    ShiftDirection_RIGHT,
} ShiftDirection;

void activePiece_newPiece(ActivePiece *active, CTetPiece piece);

bool activePiece_shift(ActivePiece *active, ShiftDirection direction);

bool activePiece_dropOneLine(ActivePiece *active);

bool activePiece_collidesWithOrientation(const ActivePiece *active, CTetOrientation orientation, CTetPoint offset);

bool activePiece_collidesWith(const ActivePiece *active, CTetPoint offset);

void activePiece_slamToFloor(ActivePiece *active, int *distanceOut);

int activePiece_getDistanceToGround(const ActivePiece *active);

void activePiece_placeToField(ActivePiece *active, double lockTimestamp);

bool activePiece_rotate(ActivePiece *active, int amount);


/** HOLD Queue **/

typedef struct {
    CTetPiece held;
    int holdCountBeforeLock;
} HoldQueue;

void holdQueue_reset(HoldQueue *holdQueue);

bool holdQueue_performHold(HoldQueue *holdQueue, CTetPiece *dest, const CTetPiece *src);

void holdQueue_onLock(HoldQueue *holdQueue);

bool holdQueue_isLocked(const HoldQueue *holdQueue);


/** NEXT Queue **/

typedef struct {
    CTetPiece pieces[5];
    uint64_t nextSeed;
    CTetPieceType bag[CTetPieceType_LENGTH];
    int remainingPiecesInBag;
} NextQueue;

void nextQueue_reset(NextQueue *nextQueue);

CTetPiece nextQueue_next(NextQueue *nextQueue);