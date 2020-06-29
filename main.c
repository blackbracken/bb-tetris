#include <ncurses.h>

#include "graphics.h"
#include "scene/title.h"

void start_marathon(int lines);

int main() {
    initscr();
    init_curses();
    getch(); // wait for alacritty.

    printw("Hello, World.");
    refresh();

    disp_menu();

    endwin();
    return 0;
}