#pragma once

typedef enum {
    BlockColor_INVALID = -1,
    BlockColor_NONE,
    BlockColor_RED,
    BlockColor_ORANGE,
    BlockColor_YELLOW,
    BlockColor_GREEN,
    BlockColor_LIGHT_BLUE,
    BlockColor_BLUE,
    BlockColor_PURPLE,
} BlockColor;

typedef struct {
    BlockColor color;
} Block;

