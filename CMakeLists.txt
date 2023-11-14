cmake_minimum_required(VERSION 3.25)

project(CTet C)
set(CMAKE_C_STANDARD 11)

set(SOURCES
        src/math/quick-math.h
        src/math/point.c
        src/math/prng.h src/math/prng.c

        src/game/engine.h src/game/engine.c

        src/game/data/piece-data.h src/game/data/piece-data.c

        src/game/component/block.h src/game/component/block.c
        src/game/component/piece.h src/game/component/piece.c
        src/game/component/active-piece.h src/game/component/active-piece.c
        src/game/component/field.h src/game/component/field.c
        src/game/component/autoshift.h src/game/component/autoshift.c
        src/game/component/gravity.h src/game/component/gravity.c
        src/game/component/next-queue.h src/game/component/next-queue.c
        src/game/component/hold-queue.h src/game/component/hold-queue.c
        src/game/component/lockdown.h src/game/component/lockdown.c
        src/game/component/wallkick.h src/game/component/wallkick.c
        include/ctet/ctet.h
)


add_library(CTet ${SOURCES})
target_include_directories(CTet PUBLIC ${PROJECT_SOURCE_DIR}/include)

