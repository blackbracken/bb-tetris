#include "title.h"

#include <string.h>
#include <ncurses.h>
#include <stdbool.h>

#include "../graphics.h"
#include "../utils.h"

const char *TEXT_TITLE = "BB_TETRIS";
const char *TEXT_ITEM_MARATHON_40 = "MARATHON 40";
const char *TEXT_ITEM_MARATHON_150 = "MARATHON 150";
const char *TEXT_ITEM_RANKING = "RANKING";
const char *TEXT_ITEM_HELP = "HELP";
const char *TEXT_ITEM_EXIT = "EXIT";

const char *DESCRIPTION_ITEM_MARATHON_40 = "Race to clear 40 lines.";
const char *DESCRIPTION_ITEM_MARATHON_150 = "Race to clear 150 lines.";
const char *DESCRIPTION_ITEM_RANKING = "See the ranking.";
const char *DESCRIPTION_ITEM_HELP = "See the instructions.";
const char *DESCRIPTION_ITEM_EXIT = "Exit the game.";

int calc_center_x_of_text(const char *text);

MenuDestination disp_menu() {
    int selected = 0;

    while (true) {
        timeout(-1);
        clear();
        draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

        // draw a title
        move(8, calc_center_x_of_text(TEXT_TITLE));
        attron(A_BOLD);
        addstr(TEXT_TITLE);
        attroff(A_BOLD);

        // draw items
        for (int item_idx = 0; item_idx < sizeof(MenuDestination) + 1; item_idx++) {
            const char *item_text;

            switch (item_idx) {
                case DEST_MARATHON_40:
                    item_text = TEXT_ITEM_MARATHON_40;
                    break;
                case DEST_MARATHON_150:
                    item_text = TEXT_ITEM_MARATHON_150;
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

            char title[strlen(item_text) + 4];
            if (item_idx == selected) {
                text_x -= 2;
                sprintf(title, "> %s", item_text);
            } else {
                strcpy(title, item_text);
            }

            move(text_y, text_x);
            addstr(title);
        }

        const char *description;
        switch (selected) {
            case DEST_MARATHON_40:
                description = DESCRIPTION_ITEM_MARATHON_40;
                break;
            case DEST_MARATHON_150:
                description = DESCRIPTION_ITEM_MARATHON_150;
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
        int desc_x = calc_center_x_of_text(description);
        int desc_y = WINDOW_HEIGHT - 3;
        move(desc_y, desc_x);
        addstr(description);

        refresh();

        int inputKey = getch();

        switch (inputKey) {
            case ' ':
            case KEY_ENTER:
                return selected;
            case 'w':
            case KEY_UP:
                selected = max(selected - 1, DEST_MARATHON_40);
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