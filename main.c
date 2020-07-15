#include <ncurses.h>

#include "graphics.h"
#include "scene/title.h"
#include "scene/marathon.h"

int main() {
    initscr();
    init_curses();

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