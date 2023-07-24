#pragma once

typedef struct Game Game;

Game *game_create();

void game_destroy(Game *game);

void game_run(Game *game);