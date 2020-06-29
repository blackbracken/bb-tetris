#include <ncurses.h>

#include "graphics.h"
#include "scene/title.h"

int main() {
    initscr();
    init_curses();

    while (1) {
        MenuDestination dest = disp_menu();

        switch (dest) {
            case DEST_MARATHON_40:
                break;
            case DEST_EXIT:
                endwin();
                return 0;
            default:
                break;
        }
    }
}