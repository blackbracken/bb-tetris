#include "help.h"

#include <ncurses.h>
#include <string.h>

#include "../graphics.h"
#include "components.h"

#define HELP_TEXT_CENTER_X WINDOW_WIDTH / 2 - 11

const char *TEXT_HELP = " -- KEY BINDINGS -- \n"
                        "\n"
                        "   [A]   : LEFT\n"
                        "   [D]   : RIGHT\n"
                        "   [W]   : HARD DROP\n"
                        "   [S]   : SOFT DROP\n"
                        " [SPACE] : HOLD\n"
                        "   [J]   : LEFT SPIN\n"
                        "   [K]   : RIGHT SPIN\n"
                        "\n"
                        "\n"
                        "\n"
                        " -- SCORE BONUSES --\n"
                        "\n"
                        " SINGLE        : 30\n"
                        " DOUBLE        : 100\n"
                        " TRIPLE        : 300\n"
                        " TETRIS        : 1200\n"
                        " TSPIN-SINGLE  : 800\n"
                        " TSPIN-DOUBLE  : 1600\n"
                        " TSPIN-TRIPLE  : 2400\n"
                        " PERFECT CLEAR : 3600\n"
                        "\n"
                        " (BtB BONUS : x2)\n";

void disp_help() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    attrset(COLOR_PAIR(COLOR_ID_HIGH));

    int text_x = HELP_TEXT_CENTER_X;
    int text_y = 6;
    for (int i = 0; i < strlen(TEXT_HELP); i++) {
        if (TEXT_HELP[i] == '\n') {
            text_x = HELP_TEXT_CENTER_X;
            text_y++;
        } else {
            mvaddch(text_y, text_x++, TEXT_HELP[i]);
        }
    }

    attrset(COLOR_PAIR(COLOR_ID_FIELD));
    mvaddstr(WINDOW_HEIGHT - 4, WINDOW_WIDTH / 2 - strlen(TEXT_RETURN) / 2, TEXT_RETURN);

    refresh();

    while (getch() != ' ');
}