#ifndef BBTETRIS_GRAPHICS_H
#define BBTETRIS_GRAPHICS_H

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const short COLOR_ID_NONE;
extern const short COLOR_ID_FIELD;
extern const short COLOR_ID_MINO_T;

void init_curses();

void draw_frame();

void efface_window();

#endif
