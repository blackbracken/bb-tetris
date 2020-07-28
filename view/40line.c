#include "40line.h"
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "../graphics.h"
#include "../tetris/game.h"
#include "components.h"

const char TARGET_LINE = 40;

void disp_40line_result(int lines, Statistics *statistics);

void start_40line() {
    int field_orig_x = WINDOW_WIDTH / 2 - 24;
    int field_orig_y = 10;

    // prepare the view
    timeout(DELAY_MILLI_PER_FRAME);
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    Board board;
    make_board(&board);

    int frame = 1;
    struct timespec delta_timespec;
    while (true) {
        timespec_get(&delta_timespec, TIME_UTC);
        long milli_start = delta_timespec.tv_nsec / 1000 / 1000;

        bool is_buried = !render(&board, frame);

        int level = board.statistics.total_removed_lines / 10 + 1;
        board.dropping_mass_per_second = level;

        draw_field(&board, field_orig_y, field_orig_x);
        draw_hold(&board, field_orig_y, field_orig_x - 2 - 6);
        draw_next(&board, field_orig_y, field_orig_x + 2 * FIELD_WIDTH + 2 + 4);
        draw_rewards(&board, field_orig_y + FIELD_HEIGHT + 1, field_orig_x);
        refresh();

        if (is_buried || board.statistics.total_removed_lines >= TARGET_LINE) {
            disp_40line_result(TARGET_LINE, &board.statistics);
            return;
        }

        int input_key = getch();
        send_input(&board, input_key);

        if (++frame % (FPS + 1) == 0) frame = 1;

        timespec_get(&delta_timespec, TIME_UTC);
        long milli_finish = delta_timespec.tv_nsec / 1000 / 1000;
        if (milli_finish < milli_start) milli_finish += 1000;

        // stabilize fps
        long milli_delta = DELAY_MILLI_PER_FRAME - (milli_finish - milli_start);
        if (input_key != ERR && milli_delta > 0) {
            usleep(milli_delta * 1000);
        }
    }
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