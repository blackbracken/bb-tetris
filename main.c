#include <ncurses.h>

void init_curses();

int main() {
    initscr();
    init_curses();
    getch(); // wait for alacritty.

    printw("Hello, World.");
    refresh();

    getch();
    endwin();

    return 0;
}

void init_curses() {
    curs_set(0);
    noecho();
    start_color();
    use_default_colors();
}