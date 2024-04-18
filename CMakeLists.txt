cmake_minimum_required(VERSION 3.25)

project(CTet C)
set(CMAKE_C_STANDARD 11)

set(SOURCES
        # Public Include
        include/ctet/ctet.h

        # Math
        src/math/math.h
        src/math/point.c
        src/math/prng.c

        # Engine
        src/game/engine.c

        # Data
        src/game/data/data.h
        src/game/data/piece-data.c
        
        # Behavior
        src/game/behavior/behavior.h
        src/game/behavior/autoshift.c
        src/game/behavior/gravity.c
        src/game/behavior/lockdown.c
        src/game/behavior/wallkick.c

        # Component
        src/game/component/component.h
        src/game/component/piece.c
        src/game/component/active-piece.c
        src/game/component/field.c
        src/game/component/next-queue.c
        src/game/component/hold-queue.c
)


add_library(CTet ${SOURCES})
target_include_directories(CTet PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(CTet PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(CTet PRIVATE ${PROJECT_SOURCE_DIR}/include-internal)

