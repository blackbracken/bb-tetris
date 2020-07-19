#ifndef BBTETRIS_GRAPHICS_H
#define BBTETRIS_GRAPHICS_H

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const short COLOR_ID_NONE;
extern const short COLOR_ID_FIELD;

typedef enum {
    RED = 21,
    WHITE = 22,
    YELLOW = 23,
    GREEN = 24,
    CYAN = 25,
    BLUE = 26,
    PURPLE = 27,
    AIR = 28,
} MinoBlock;

void init_curses();

void draw_window_frame();

void efface_window();

short to_color_id(MinoBlock block);

#endif
