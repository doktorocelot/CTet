cmake_minimum_required(VERSION 3.25)

option(OCELOT_CONTROLS OFF)
if (OCELOT_CONTROLS)
    add_compile_definitions(OCELOT_CONTROLS)
endif()

project(CTet C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SDL2 REQUIRED)
include_directories(CTet ${SDL2_INCLUDE_DIRS})


file(COPY extern/SDL2.dll DESTINATION ${CMAKE_BINARY_DIR}/bin)

set(SOURCES
        src/main.c

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
        )

add_executable(CTet WIN32 ${SOURCES})
target_link_libraries(CTet ${SDL2_LIBRARIES})

