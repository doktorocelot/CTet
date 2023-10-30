#define SDL_MAIN_HANDLED

#include "../window.h"

//#include "../../game/game.h"
#include <Windows.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    Window *window = window_create();
    window_show(window);
    window_loop(window);
    window_destroy(window);
    
//    Game *game = game_create();
//    game_run(game);
//    game_destroy(game);

    return 0;
}
