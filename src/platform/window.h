#pragma once

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define WINDOW_TITLE "CTet"
typedef struct Window Window;

Window *window_create();

void window_show(Window *window);

void window_loop(Window *window);

void window_destroy(Window *window);