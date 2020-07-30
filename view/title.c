#include "title.h"

#include <string.h>
#include <ncurses.h>
#include <stdbool.h>

#include "../graphics.h"
#include "../utils.h"

const char *TEXT_TITLE_AA_1 = ".__ .__   .___..___.___..__ ._. __.";
const char *TEXT_TITLE_AA_2 = "[__)[__)    |  [__   |  [__) | (__ ";
const char *TEXT_TITLE_AA_3 = "[__)[__)    |  [___  |  |  \\_|_.__)";

const char *TEXT_TITLE_SPLITTER = "+----------------------------------------+";

const char *TEXT_ITEM_40LINE = "PLAY 40LINE";
const char *TEXT_ITEM_MARATHON = "PLAY MARATHON";
const char *TEXT_ITEM_ULTRA = "PLAY ULTRA";
const char *TEXT_ITEM_RANKING = "RANKING";
const char *TEXT_ITEM_HELP = "HELP";
const char *TEXT_ITEM_EXIT = "EXIT";

const char *DESCRIPTION_ITEM_40LINE = "Race to clear 40 lines.";
const char *DESCRIPTION_ITEM_MARATHON = "Race a score by clearing 150 lines.";
const char *DESCRIPTION_ITEM_ULTRA = "Race a score in 3 minutes.";
const char *DESCRIPTION_ITEM_RANKING = "See the ranking.";
const char *DESCRIPTION_ITEM_HELP = "See the instructions.";
const char *DESCRIPTION_ITEM_EXIT = "Exit the game.";

int calc_center_x_of_text(const char *text);

MenuDestination disp_menu() {
    int selected = DEST_40LINE;

    while (true) {
        timeout(-1);
        erase();
        draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

        attrset(COLOR_PAIR(COLOR_ID_FIELD));
        mvaddstr(8, calc_center_x_of_text(TEXT_TITLE_AA_1), TEXT_TITLE_AA_1);
        mvaddstr(9, calc_center_x_of_text(TEXT_TITLE_AA_2), TEXT_TITLE_AA_2);
        mvaddstr(10, calc_center_x_of_text(TEXT_TITLE_AA_3), TEXT_TITLE_AA_3);

        attrset(COLOR_PAIR(COLOR_ID_PLAIN));
        mvaddstr(13, calc_center_x_of_text(TEXT_TITLE_SPLITTER), TEXT_TITLE_SPLITTER);

        for (int item_idx = 0; item_idx < NUM_OF_DEST; item_idx++) {
            const char *item_text;

            switch (item_idx) {
                case DEST_40LINE:
                    item_text = TEXT_ITEM_40LINE;
                    break;
                case DEST_MARATHON:
                    item_text = TEXT_ITEM_MARATHON;
                    break;
                case DEST_ULTRA:
                    item_text = TEXT_ITEM_ULTRA;
                    break;
                case DEST_RANKING:
                    item_text = TEXT_ITEM_RANKING;
                    break;
                case DEST_HELP:
                    item_text = TEXT_ITEM_HELP;
                    break;
                case DEST_EXIT:
                    item_text = TEXT_ITEM_EXIT;
                    break;
                default:
                    item_text = "unreachable";
                    break;
            }

            int text_x = calc_center_x_of_text(item_text);
            int text_y = 2 * item_idx + 16;

            char title[strlen(item_text) + 8];
            if (item_idx == selected) {
                text_x -= 3;
                sprintf(title, "-[ %s ]-", item_text);
                attron(A_BOLD);
            } else {
                strcpy(title, item_text);
            }

            mvaddstr(text_y, text_x, title);
            attroff(A_BOLD);
        }

        const char *description;
        switch (selected) {
            case DEST_40LINE:
                description = DESCRIPTION_ITEM_40LINE;
                break;
            case DEST_MARATHON:
                description = DESCRIPTION_ITEM_MARATHON;
                break;
            case DEST_ULTRA:
                description = DESCRIPTION_ITEM_ULTRA;
                break;
            case DEST_RANKING:
                description = DESCRIPTION_ITEM_RANKING;
                break;
            case DEST_HELP:
                description = DESCRIPTION_ITEM_HELP;
                break;
            case DEST_EXIT:
                description = DESCRIPTION_ITEM_EXIT;
                break;
            default:
                description = "unreachable";
                break;
        }
        mvaddstr(WINDOW_HEIGHT - 3, calc_center_x_of_text(description), description);

        refresh();

        int inputKey = getch();

        switch (inputKey) {
            case ' ':
            case KEY_ENTER:
                return (MenuDestination) selected;
            case 'w':
            case KEY_UP:
                selected = max(selected - 1, DEST_40LINE);
                break;
            case 's':
            case KEY_DOWN:
                selected = min(selected + 1, DEST_EXIT);
                break;
            default:
                break;
        }
    }
}

int calc_center_x_of_text(const char *text) {
    return WINDOW_WIDTH / 2 - (int) strlen(text) / 2;
}