#include <Windows.h>
#include <ctetdx.h>

#include "../window.h"

LRESULT CALLBACK windowProcedure(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CLOSE:
            DestroyWindow(window);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(window, msg, wParam, lParam);
    }
    return 0;
}

struct Window {
    HINSTANCE instance;
    LPCSTR className;
    HWND window;
    DxRenderer dxR;
};


Window *window_create() {
    Window *window = malloc(sizeof(Window));

    HINSTANCE instance = GetModuleHandle(NULL);

    window->className = "MainWindowClass";

    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = windowProcedure;
    wc.hInstance = instance;
    wc.hbrBackground = (HBRUSH) (COLOR_BACKGROUND);
    wc.lpszClassName = window->className;
    RegisterClassEx(&wc);

    window->instance = instance;

    window->window = CreateWindowEx(
            0,
            window->className,
            WINDOW_TITLE,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT,
            NULL, NULL, instance, NULL
    );

    if (window->window == NULL) {
        exit(-1);
    }

    window->dxR = (DxRenderer) {0};
    dxRenderer_init(&window->dxR, window->window, SCREEN_WIDTH, SCREEN_HEIGHT);

    return window;
}

void window_show(Window *window) {
    ShowWindow(window->window, SW_SHOWNORMAL);
    UpdateWindow(window->window);
}

void window_loop(Window *window) {
    dxRenderer_renderFrame(&window->dxR);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void window_destroy(Window *window) {
    dxRenderer_cleanup(&window->dxR);
    UnregisterClass(window->className, window->instance);
    DestroyWindow(window->window);
    free(window);
}


