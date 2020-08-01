#ifndef BBTETRIS_GAME_H
#define BBTETRIS_GAME_H

#include <stdbool.h>

#include "tetrimino.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 21
#define NEXT_AMOUNT 5

extern const int FPS;
extern const int DELAY_MILLI_PER_FRAME;

typedef struct {
    int score;
    int total_removed_lines;
    int elapsed_seconds;
} Statistics;

typedef struct {
    const Tetrimino *blocks[7 * 2];
    int head, done_tail;
} MinoBag;

typedef enum {
    NONE,
    TETRIS,
    TSPIN_SINGLE,
    TSPIN_DOUBLE,
    TSPIN_TRIPLE,
    PERFECT_CLEAR,
} RemovingReward;

typedef struct {
    int frame;
    MinoBlock field[FIELD_HEIGHT][FIELD_WIDTH];
    const Tetrimino *dropping_mino;
    const Tetrimino *held_mino;
    bool did_already_hold;
    int dropping_mino_x, dropping_mino_y;
    int dropping_mino_spin;
    int dropping_mass_per_second;
    int lockdown_count;
    MinoBag bag;
    Statistics statistics;
    int ren_count;
    RemovingReward removing_reward;
    bool on_ready_back_to_back, did_back_to_back;
    bool on_immediately_after_rotating;
} Board;

void make_board(Board *board);

bool render(Board *board, int frame);

const Tetrimino *peek_next(Board *board, int order);

void try_spin_left(Board *board);

void try_spin_right(Board *board);

void try_move_left(Board *board);

void try_move_right(Board *board);

void drop_softly(Board *board);

void drop_hardly(Board *board);

void try_hold(Board *board);

#endif
