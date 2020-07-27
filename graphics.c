#include "graphics.h"

#include <ncurses.h>

const int WINDOW_WIDTH = 80;
const int WINDOW_HEIGHT = 40;

const short COLOR_ID_NONE = 64;
const short COLOR_ID_FIELD = 65;
const short COLOR_ID_WINDOW = 66;
const short COLOR_ID_PLAIN = 67;
const short COLOR_ID_SUCCESS = 68;
const short COLOR_ID_FAILURE = 69;

void init_curses() {
    curs_set(0);
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    init_pair(COLOR_ID_NONE, -1, -1);
    init_pair(COLOR_ID_FIELD, COLOR_CYAN, -1);
    init_pair(COLOR_ID_WINDOW, COLOR_WHITE, -1);
    init_pair(COLOR_ID_PLAIN, COLOR_WHITE, -1);
    init_pair(COLOR_ID_SUCCESS, COLOR_CYAN, -1);
    init_pair(COLOR_ID_FAILURE, COLOR_YELLOW, -1);
    init_pair(to_color_id(RED), -1, COLOR_RED);
    init_pair(to_color_id(WHITE), -1, COLOR_WHITE);
    init_pair(to_color_id(YELLOW), -1, COLOR_YELLOW);
    init_pair(to_color_id(GREEN), -1, COLOR_GREEN);
    init_pair(to_color_id(CYAN), -1, COLOR_CYAN);
    init_pair(to_color_id(BLUE), -1, COLOR_BLUE);
    init_pair(to_color_id(PURPLE), -1, COLOR_MAGENTA);
}

void draw_window_frame(int start_y, int start_x, int end_y, int end_x) {
    attrset(COLOR_PAIR(COLOR_ID_WINDOW));

    for (int y = start_y; y < end_y - 1; y++) {
        mvaddstr(y, start_x, "[]");
        mvaddstr(y, end_x - 2, "[]");
    }
    for (int x = start_x; x < end_x; x++) {
        mvaddch(start_y, x, '=');
        mvaddch(end_y - 1, x, '=');
    }
}

short to_color_id(MinoBlock block) {
    return (short) block;
}