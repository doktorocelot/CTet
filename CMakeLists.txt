cmake_minimum_required(VERSION 3.25)

project(CTet C)
set(CMAKE_C_STANDARD 11)

set(SOURCES
        src/math/quick-math.h
        src/math/point.h src/math/point.c
        src/math/prng.h src/math/prng.c

        src/game/game.h src/game/game.c

        src/game/engine/engine.h src/game/engine/engine.c

        src/game/engine/data/piece-data.h src/game/engine/data/piece-data.c

        src/game/engine/component/block.h src/game/engine/component/block.c
        src/game/engine/component/piece.h src/game/engine/component/piece.c
        src/game/engine/component/active-piece.h src/game/engine/component/active-piece.c
        src/game/engine/component/field.h src/game/engine/component/field.c
        src/game/engine/component/autoshift.h src/game/engine/component/autoshift.c
        src/game/engine/component/gravity.h src/game/engine/component/gravity.c
        src/game/engine/component/next-queue.h src/game/engine/component/next-queue.c
        src/game/engine/component/hold-queue.h src/game/engine/component/hold-queue.c
        src/game/engine/component/lockdown.h src/game/engine/component/lockdown.c
        src/game/engine/component/wallkick.h src/game/engine/component/wallkick.c
)


add_library(CTet ${SOURCES})


