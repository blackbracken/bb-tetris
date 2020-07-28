#include "40line.h"
#include <ncurses.h>
#include <string.h>

#include "../graphics.h"
#include "../tetris/game.h"
#include "components.h"

#define FIELD_ORIG_X (WINDOW_WIDTH / 2 - 24)
#define FIELD_ORIG_Y 10

const char TARGET_LINE = 40;
const char *TEXT_MODE_40LINE = "40LINE";

bool render_frame_of_40line(Board *board, int frame);

void disp_40line_result(int lines, Statistics *statistics);

void start_40line() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    Board board;
    make_board(&board);
    start_routine(&board, render_frame_of_40line);

    disp_40line_result(TARGET_LINE, &board.statistics);
}

bool render_frame_of_40line(Board *board, int frame) {
    bool is_buried = !render(board, frame);

    int level = board->statistics.total_removed_lines / 10 + 1;
    board->dropping_mass_per_second = level;

    draw_field(board, FIELD_ORIG_Y, FIELD_ORIG_X);
    draw_hold(board, FIELD_ORIG_Y, FIELD_ORIG_X - 2 - 6);
    draw_next(board, FIELD_ORIG_Y, FIELD_ORIG_X + 2 * FIELD_WIDTH + 2 + 4);
    draw_rewards(board, FIELD_ORIG_Y + FIELD_HEIGHT + 1, FIELD_ORIG_X);
    draw_stats(board, TEXT_MODE_40LINE, FIELD_ORIG_Y + 4, FIELD_ORIG_X + 2 * FIELD_WIDTH + 16);
    refresh();

    return !is_buried && board->statistics.total_removed_lines < TARGET_LINE;
}

void disp_40line_result(int lines, Statistics *statistics) {
    int start_x = WINDOW_WIDTH / 4;
    int start_y = WINDOW_HEIGHT / 3;
    int end_x = WINDOW_WIDTH / 4 * 3;
    int end_y = WINDOW_HEIGHT / 3 * 2;
    int center_x = end_x / 2 + start_x / 2;

    draw_window_frame(start_y - 1, start_x - 2, end_y + 1, end_x + 2);

    attrset(COLOR_PAIR(COLOR_ID_NONE));
    for (int j = start_y; j < end_y; j++) {
        for (int i = start_x; i < end_x; i++) {
            mvaddch(j, i, ' ');
        }
    }

    bool is_succeed = statistics->total_removed_lines >= lines;

    char record[64];
    if (is_succeed) {
        attrset(COLOR_PAIR(COLOR_ID_SUCCESS));
        mvaddstr(start_y + 1, center_x - strlen(TEXT_SUCCESS) / 2, TEXT_SUCCESS);
        sprintf(record, "record: [ %02d:%02d ]", statistics->elapsed_seconds / 60, statistics->elapsed_seconds % 60);
    } else {
        attrset(COLOR_PAIR(COLOR_ID_FAILURE));
        mvaddstr(start_y + 1, center_x - strlen(TEXT_FAILURE) / 2, TEXT_FAILURE);
        sprintf(record, "cleared: %d lines", statistics->total_removed_lines);
    }

    attrset(COLOR_PAIR(COLOR_ID_PLAIN));
    mvaddstr(start_y + 3, center_x - strlen(record) / 2, record);
    mvaddstr(end_y - 2, center_x - strlen(TEXT_RETURN) / 2, TEXT_RETURN);

    refresh();

    while (getch() != ' ');
}