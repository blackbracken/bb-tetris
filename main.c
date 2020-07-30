#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "graphics.h"
#include "view/title.h"
#include "view/40line.h"
#include "view/marathon.h"
#include "view/ultra.h"
#include "ranking.h"

int main() {
    WINDOW *window = initscr();
    init_curses();
    init_ranking();
    srand(time(NULL));

    if (wresize(window, WINDOW_HEIGHT, WINDOW_WIDTH) == ERR) {
        fputs("A window size must be equal to or wider than 80x40.\n", stderr);
        return 1;
    }

    while (true) {
        MenuDestination dest = disp_menu();

        switch (dest) {
            case DEST_40LINE:
                start_40line();
                break;
            case DEST_MARATHON:
                start_marathon();
                break;
            case DEST_ULTRA:
                start_ultra();
                break;
            case DEST_EXIT:
                endwin();
                return 0;
            default:
                break;
        }
    }
}

#pragma clang diagnostic push