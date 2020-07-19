#ifndef BBTETRIS_GRAPHICS_H
#define BBTETRIS_GRAPHICS_H

#include "tetris/tetrimino.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const short COLOR_ID_NONE;
extern const short COLOR_ID_FIELD;

void init_curses();

void draw_window_frame();

void efface_window();

short to_color_id(MinoBlock block);

#endif
