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

int calc_center_x_of_text(const char *text);

void put_selected_str(int y, int x, const char *text);

MenuDestination disp_menu() {
    int selected = 0;

    while (true) {
        efface_window();
        draw_window_frame();

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

            int x = calc_center_x_of_text(item_text);
            int y = 2 * item_idx + 16;
            if (item_idx == selected) {
                put_selected_str(y, x, item_text);
            } else {
                move(y, x);
                addstr(item_text);
            }
        }

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
    return WINDOW_WIDTH / 2 - strlen(text) / 2;
}

void put_selected_str(int y, int x, const char *const text) {
    move(y, x - 2);
    attron(A_BOLD);
    addstr("> ");
    attroff(A_BOLD);
    addstr(text);
}