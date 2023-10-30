#include "../log.h"

#include <Windows.h>

void log(char* msg) {
    OutputDebugStringA(msg);
}