#include "title.h"

#include <string.h>
#include <ncurses.h>

#include "../graphics.h"

// TODO: separate as utilities
#define max(x, y) (x > y ? x : y)
#define min(x, y) (x < y ? x : y)

#define MENU_ITEM_MARATHON_40   0
#define MENU_ITEM_MARATHON_150  1
#define MENU_ITEM_RANKING       2
#define MENU_ITEM_HELP          3
#define MENU_ITEM_EXIT          4
#define MENU_ITEM_AMOUNT        5

const char *TEXT_TITLE = "BB_TETRIS";
const char *TEXT_ITEM_MARATHON_40 = "MARATHON 40";
const char *TEXT_ITEM_MARATHON_150 = "MARATHON 150";
const char *TEXT_ITEM_RANKING = "RANKING";
const char *TEXT_ITEM_HELP = "HELP";
const char *TEXT_ITEM_EXIT = "EXIT";

int on_select_menu_item(int selected);

int calc_center_x_of_text(char *text);

void put_selected_str(int y, int x, const char *text);

void disp_menu() {
    int selected = 0;

    while (true) {
        efface_window();
        { // draw a frame
            for (int y = 1; y < WINDOW_HEIGHT - 1; y++) {
                move(y, 0);
                addstr("[]");
            }
            for (int y = 1; y < WINDOW_HEIGHT - 1; y++) {
                move(y, WINDOW_WIDTH - 2);
                addstr("[]");
            }
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                move(0, x);
                addch('=');
            }
            for (int x = 0; x < WINDOW_WIDTH; x++) {
                move(WINDOW_HEIGHT - 1, x);
                addch('=');
            }
        }

        // draw a title
        move(8, calc_center_x_of_text(TEXT_TITLE));
        attron(A_BOLD);
        addstr(TEXT_TITLE);
        attroff(A_BOLD);

        // draw items
        for (int item_idx = 0; item_idx < MENU_ITEM_AMOUNT; item_idx++) {
            const char *item_text;
            switch (item_idx) {
                case MENU_ITEM_MARATHON_40:
                    item_text = TEXT_ITEM_MARATHON_40;
                    break;
                case MENU_ITEM_MARATHON_150:
                    item_text = TEXT_ITEM_MARATHON_150;
                    break;
                case MENU_ITEM_RANKING:
                    item_text = TEXT_ITEM_RANKING;
                    break;
                case MENU_ITEM_HELP:
                    item_text = TEXT_ITEM_HELP;
                    break;
                case MENU_ITEM_EXIT:
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
                // whether to continue
                if (on_select_menu_item(selected)) {
                    selected = MENU_ITEM_MARATHON_40;
                } else {
                    return;
                }
                break;
            case 'w':
            case KEY_UP:
                selected = max(selected - 1, 0);
                break;
            case 's':
            case KEY_DOWN:
                selected = min(selected + 1, MENU_ITEM_AMOUNT - 1);
                break;
            default:
                break;
        }
    }
}

int calc_center_x_of_text(char *text) {
    return WINDOW_WIDTH / 2 - strlen(text) / 2;
}

void put_selected_str(int y, int x, const char *text) {
    move(y, x - 2);
    attron(A_BOLD);
    addstr("> ");
    attroff(A_BOLD);
    addstr(text);
}

int on_select_menu_item(int selected) {
    switch (selected) {
        case MENU_ITEM_MARATHON_40:
//            start_marathon(40);
            break;
        case MENU_ITEM_MARATHON_150:
            break;
        case MENU_ITEM_RANKING:
            break;
        case MENU_ITEM_HELP:
            break;
        case MENU_ITEM_EXIT:
            return 0;
        default:
            break;
    }

    return 1;
}