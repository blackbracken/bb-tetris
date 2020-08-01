#include "40line.h"
#include <ncurses.h>
#include <string.h>

#include "../graphics.h"
#include "../tetris/game.h"
#include "components.h"

const char *TEXT_MODE_40LINE = "40LINE";
const int TARGET_LINE_40LINE = 40;

bool render_frame_of_40line(Board *board, int frame);

void record_40line(char *name, Statistics *statistics);

void start_40line() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    Board board;
    make_board(&board);

    start_routine(&board, render_frame_of_40line);

    GameResult result;
    char record[32];
    if (board.statistics.total_removed_lines >= TARGET_LINE_40LINE) {
        result = is_new_40line_record(board.statistics.elapsed_seconds) ? RES_NEW_RECORD : RES_SUCCESS;
        sprintf(record, "%02d:%02d", board.statistics.elapsed_seconds / 60, board.statistics.elapsed_seconds % 60);
    } else {
        result = RES_FAILURE;
        sprintf(record, "%d lines", board.statistics.total_removed_lines);
    }

    show_result_and_wait(result, record, &board.statistics, record_40line);
}

bool render_frame_of_40line(Board *board, int frame) {
    int field_orig_x = WINDOW_WIDTH / 2 - 24;
    int field_orig_y = 10;

    bool is_buried = !render(board, frame);

    draw_field(board, field_orig_y, field_orig_x);
    draw_hold(board, field_orig_y, field_orig_x - 2 - 6);
    draw_next(board, field_orig_y, field_orig_x + 2 * FIELD_WIDTH + 2 + 4);
    draw_rewards(board, field_orig_y + FIELD_HEIGHT + 1, field_orig_x);
    draw_stats(board, TEXT_MODE_40LINE, field_orig_y + 4, field_orig_x + 2 * FIELD_WIDTH + 16);
    refresh();

    return !is_buried && board->statistics.total_removed_lines < TARGET_LINE_40LINE;
}

void record_40line(char *name, Statistics *statistics) {
    TimeRecord rec;
    strcpy(rec.name, name);
    rec.seconds = statistics->elapsed_seconds;

    insert_40line_record(&rec);
}