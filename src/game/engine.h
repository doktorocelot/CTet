#pragma once

#include <ctet/ctet.h>

#include "component/active-piece.h"
#include "component/lockdown.h"
#include "component/hold-queue.h"
#include "component/next-queue.h"
#include "component/gravity.h"
#include "component/autoshift.h"

struct CTetEngine {
    Field field;
    AutoshiftVars autoshiftVars;
    Gravity gravity;
    NextQueue nextQueue;
    HoldQueue holdQueue;
    Lockdown lockdown;
    ActivePiece active;
    bool isDead;
};



void engine_spawnNewPiece(CTetEngine *engine, CTetPiece piece);

bool engine_placingPieceWillDie(CTetEngine *engine);