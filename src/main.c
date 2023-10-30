#define SDL_MAIN_HANDLED

#include "platform/window.h"

#include "game/game.h"

#ifdef __WIN32__

#include <windows.h>
#include <ctetdx.h>

#endif

#ifdef __WIN32__
#ifndef USE_WIN_CONSOLE
    int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#else
    int main(int argc, char *argv[]) {
#endif
#else
    int main(int argc, char *argv[]) {
#endif
        
        

        Window *window = window_create();
        window_show(window);
        window_loop(window);
        window_destroy(window);

//    Game *game = game_create();
//    game_run(game);
//    game_destroy(game);

    return 0;
}
