#include <ncurses.h>
#include <string.h>
#include "result_marathon.h"
#include "../graphics.h"

#define WINDOW_START_X WINDOW_WIDTH / 4
#define WINDOW_START_Y WINDOW_HEIGHT / 3
#define WINDOW_END_X WINDOW_WIDTH / 4 * 3
#define WINDOW_END_Y WINDOW_HEIGHT / 3 * 2
#define WINDOW_CENTER_X WINDOW_END_X / 2 + WINDOW_START_X / 2

const char *TEXT_SUCCESS = "SUCCESS!";
const char *TEXT_FAILURE = "FAILURE...";
const char *TEXT_RETURN = "PRESS SPACE KEY TO RETURN MENU";

void disp_marathon_result(int lines, int record_seconds, Statistics *statistics) {
    draw_window_frame(WINDOW_START_Y - 1, WINDOW_START_X - 2, WINDOW_END_Y + 1, WINDOW_END_X + 2);

    attrset(COLOR_PAIR(COLOR_ID_NONE));
    for (int j = WINDOW_START_Y; j < WINDOW_END_Y; j++) {
        for (int i = WINDOW_START_X; i < WINDOW_END_X; i++) {
            move(j, i);
            addch(' ');
        }
    }

    bool is_succeed = statistics->total_removed_lines >= lines;

    char record[64];
    if (is_succeed) {
        move(WINDOW_START_Y + 1, WINDOW_CENTER_X - strlen(TEXT_SUCCESS) / 2);
        attrset(COLOR_PAIR(COLOR_ID_SUCCESS));
        addstr(TEXT_SUCCESS);
        sprintf(record, "record: [ %02d:%02d ]", record_seconds / 60, record_seconds % 60);
    } else {
        move(WINDOW_START_Y + 1, WINDOW_CENTER_X - strlen(TEXT_FAILURE) / 2);
        attrset(COLOR_PAIR(COLOR_ID_FAILURE));
        addstr(TEXT_FAILURE);
        sprintf(record, "cleared: %d lines", statistics->total_removed_lines);
    }
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    move(WINDOW_START_Y + 3, WINDOW_CENTER_X - strlen(record) / 2);
    addstr(record);

    move(WINDOW_END_Y - 2, WINDOW_CENTER_X - strlen(TEXT_RETURN) / 2);
    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    addstr(TEXT_RETURN);

    refresh();

    while (getch() != ' ');
}