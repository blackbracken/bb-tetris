#include "graphics.h"

#include <ncurses.h>

const int WINDOW_WIDTH = 80;
const int WINDOW_HEIGHT = 40;

const short COLOR_ID_NONE = 0;
const short COLOR_ID_FIELD = 1;
const short COLOR_ID_MINO_T = 2;

void init_curses() {
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    init_pair(COLOR_ID_NONE, -1, -1);
    init_pair(COLOR_ID_FIELD, COLOR_CYAN, -1);
    init_pair(COLOR_ID_MINO_T, -1, COLOR_MAGENTA);
}

void draw_frame() {
    for (int y = 1; y < WINDOW_HEIGHT - 1; y++) {
        move(y, 0);
        addstr("[]");
    }
    for (int y = 1; y < WINDOW_HEIGHT - 1; y++) {
        move(y, WINDOW_WIDTH - 2);
        addstr("[]");
    }
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        move(0, x);
        addch('=');
    }
    for (int x = 0; x < WINDOW_WIDTH; x++) {
        move(WINDOW_HEIGHT - 1, x);
        addch('=');
    }
}

void efface_window() {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            move(y, x);
            attrset(COLOR_PAIR(COLOR_ID_NONE));
            addch(' ');
        }
    }
}