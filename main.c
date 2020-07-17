#include <ncurses.h>

#include "graphics.h"
#include "scene/title.h"
#include "scene/marathon.h"

int main() {
    WINDOW *window = initscr();
    init_curses();

    if (wresize(window, WINDOW_HEIGHT, WINDOW_WIDTH) == ERR) {
        fputs("A window size must be equal to or wider than 80x40.\n", stderr);
        return 1;
    }

    while (true) {
        MenuDestination dest = disp_menu();

        switch (dest) {
            case DEST_MARATHON_40:
                start_marathon(40);
                break;
            case DEST_EXIT:
                endwin();
                return 0;
            default:
                break;
        }
    }
}