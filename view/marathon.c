#include "marathon.h"

#include <ncurses.h>
#include <string.h>

#include "components.h"
#include "../graphics.h"
#include "../utils.h"

const char *TEXT_MODE_MARATHON = "MARATHON";
const int TARGET_LINE_MARATHON = 150;

bool render_frame_of_marathon(Board *board, int frame);

int calc_level(int total_removed_lines);

void expand_stats_for_draw_level(Board *board, int stats_y, int stats_x);

void record_marathon(char *name, Statistics *statistics);

void start_marathon() {
    erase();
    draw_window_frame(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH);

    Board board;
    make_board(&board);

    start_routine(&board, render_frame_of_marathon);

    GameResult result;
    char record[32];
    if (board.statistics.total_removed_lines >= TARGET_LINE_MARATHON) {
        result = is_new_marathon_record(board.statistics.score) ? RES_NEW_RECORD : RES_SUCCESS;
        sprintf(record, "%d", board.statistics.score);
    } else {
        result = RES_FAILURE;
        sprintf(record, "%d lines", board.statistics.total_removed_lines);
    }

    show_result_and_wait(result, record, &board.statistics, record_marathon);
}

bool render_frame_of_marathon(Board *board, int frame) {
    int field_orig_x = WINDOW_WIDTH / 2 - 24;
    int field_orig_y = 10;

    bool is_buried = !render(board, frame);

    int level = calc_level(board->statistics.total_removed_lines);
    board->dropping_mass_per_second = level;

    draw_field(board, field_orig_y, field_orig_x);
    draw_hold(board, field_orig_y, field_orig_x - 2 - 6);
    draw_next(board, field_orig_y, field_orig_x + 2 * FIELD_WIDTH + 2 + 4);
    draw_rewards(board, field_orig_y + FIELD_HEIGHT + 1, field_orig_x);

    int stats_x = field_orig_x + 2 * FIELD_WIDTH + 16;
    int stats_y = field_orig_y + 4;
    draw_stats(board, TEXT_MODE_MARATHON, stats_y, stats_x);
    expand_stats_for_draw_level(board, stats_y, stats_x);
    refresh();

    return !is_buried && board->statistics.total_removed_lines < TARGET_LINE_MARATHON;
}

int calc_level(int total_removed_lines) {
    return min(total_removed_lines / 10 + 1, 20);
}

void expand_stats_for_draw_level(Board *board, int stats_y, int stats_x) {
    draw_window_frame(stats_y, stats_x, stats_y + 11 + 2, stats_x + 23);

    for (int i = 2; i < 21; i++) {
        mvaddch(stats_y + 10, stats_x + i, ' ');
    }

    mvprintw(stats_y + 10, stats_x + 3, "* LEVEL: %d", calc_level(board->statistics.total_removed_lines));
}

void record_marathon(char *name, Statistics *statistics) {
    ScoreRecord rec;
    strcpy(rec.name, name);
    rec.score = statistics->score;

    insert_marathon_record(&rec);
}