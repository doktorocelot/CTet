cmake_minimum_required(VERSION 3.25)

project(CTet C)
set(CMAKE_C_STANDARD 11)

set(SOURCES
        src/math/quick-math.h
        include/ctet/point.h src/math/point.c
        include/ctet/prng.h src/math/prng.c

        include/ctet/engine.h src/game/engine/engine.c

        include/ctet/piece-data.h src/game/engine/data/piece-data.c

        include/ctet/block.h src/game/engine/component/block.c
        include/ctet/piece.h src/game/engine/component/piece.c
        include/ctet/active-piece.h src/game/engine/component/active-piece.c
        include/ctet/field.h src/game/engine/component/field.c
        include/ctet/autoshift.h src/game/engine/component/autoshift.c
        include/ctet/gravity.h src/game/engine/component/gravity.c
        include/ctet/next-queue.h src/game/engine/component/next-queue.c
        include/ctet/hold-queue.h src/game/engine/component/hold-queue.c
        include/ctet/lockdown.h src/game/engine/component/lockdown.c
        include/ctet/wallkick.h src/game/engine/component/wallkick.c
)


add_library(CTet ${SOURCES})
target_include_directories(CTet PUBLIC ${PROJECT_SOURCE_DIR}/include)

