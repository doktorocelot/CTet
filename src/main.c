#include <SDL.h>
#include "game/game.h"

int main(int argc, char *argv[]) {

    Game* game = game_create();
    game_run(game);
    game_destroy(game);

    return 0;
}
