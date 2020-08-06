#include "ultra.h"

#include <ncurses.h>
#include <string.h>

#include "../graphics.h"
#include "../tetris/game.h"
#include "components.h"

const char *TEXT_MODE_ULTRA = "ULTRA";
const int TIMER_SECONDS = 3 * 60;

bool render_frame_of_ultra(Board *board, int frame);

void record_ultra(char *name, Statistics *statistics);

void start_ultra() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    Board board;
    make_board(&board);

    repeat_routine_regularly(&board, render_frame_of_ultra);

    GameResult result;
    char record[32];
    if (board.statistics.elapsed_seconds >= TIMER_SECONDS) {
        result = is_new_ultra_records(board.statistics.score) ? RES_NEW_RECORD : RES_SUCCESS;
        sprintf(record, "%d", board.statistics.score);
    } else {
        result = RES_FAILURE;
        sprintf(record, "%d lines", board.statistics.total_removed_lines);
    }

    show_result_and_wait(result, record, &board.statistics, record_ultra);
}

bool render_frame_of_ultra(Board *board, int frame) {
    int field_orig_x = WINDOW_WIDTH / 2 - 24;
    int field_orig_y = 10;

    bool is_buried = !render(board, frame);

    draw_field(board, field_orig_y, field_orig_x);
    draw_hold(board, field_orig_y, field_orig_x - 2 - 6);
    draw_next(board, field_orig_y, field_orig_x + 2 * FIELD_WIDTH + 2 + 4);
    draw_rewards(board, field_orig_y + FIELD_HEIGHT + 1, field_orig_x);
    draw_stats(board, TEXT_MODE_ULTRA, field_orig_y + 4, field_orig_x + 2 * FIELD_WIDTH + 16);
    refresh();

    return !is_buried && board->statistics.elapsed_seconds < TIMER_SECONDS;
}

void record_ultra(char *name, Statistics *statistics) {
    ScoreRecord rec;
    strcpy(rec.name, name);
    rec.score = statistics->score;

    insert_ultra_record(&rec);
}