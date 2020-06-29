#include "graphics.h"

#include <ncurses.h>

const int WINDOW_WIDTH = 80;
const int WINDOW_HEIGHT = 40;

const short COLOR_ID_NONE = 0;

void init_curses() {
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    init_pair(COLOR_ID_NONE, -1, -1);
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