#ifndef BBTETRIS_GRAPHICS_H
#define BBTETRIS_GRAPHICS_H

#include "tetris/tetrimino.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const short COLOR_ID_NONE;
extern const short COLOR_ID_FIELD;
extern const short COLOR_ID_WINDOW;
extern const short COLOR_ID_PLAIN;
extern const short COLOR_ID_SUCCESS;
extern const short COLOR_ID_FAILURE;

void init_curses();

void draw_window_frame(int start_y, int start_x, int end_y, int end_x);

short to_color_id(MinoBlock block);

#endif
