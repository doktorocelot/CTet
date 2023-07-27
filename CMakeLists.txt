cmake_minimum_required(VERSION 3.25)

option(USE_WIN_CONSOLE OFF)
if (USE_WIN_CONSOLE)
    add_compile_definitions(USE_WIN_CONSOLE)
endif ()
option(OCELOT_CONTROLS OFF)
if (OCELOT_CONTROLS)
    add_compile_definitions(OCELOT_CONTROLS)
endif ()

project(CTet C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BUILD_SHARED_LIBS ON)
set(__WIN32__ ON)
if (WIN32)
    add_definitions(-D__WIN32__)
endif ()
add_subdirectory(extern/SDL)


set(SOURCES
        src/main.c

        src/math/quick-math.h
        src/math/point.h src/math/point.c
        src/math/prng.h src/math/prng.c

        src/game/game.h src/game/game.c

        src/game/engine/engine.h src/game/engine/engine.c
        src/game/engine/renderer.h src/game/engine/renderer.c

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

if (WIN32 AND NOT USE_WIN_CONSOLE)
    add_executable(CTet WIN32 ${SOURCES})
else ()
    add_executable(CTet ${SOURCES})
endif ()

target_include_directories(CTet PUBLIC lib/SDL-release-2.28.1/include)
target_link_libraries(CTet PUBLIC SDL2)
