#ifndef BBTETRIS_GAME_H
#define BBTETRIS_GAME_H

#include <stdbool.h>
#include "tetrimino.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 21
#define NEXT_AMOUNT 5

typedef struct {
    bool is_available;
    int score;
    int total_removed_lines;
} Result;

typedef struct {
    Tetrimino const *blocks[7 * 2];
    int order;
} MinoSeed;

typedef struct {
    int frame;
    MinoBlock field[FIELD_HEIGHT][FIELD_WIDTH];
    Tetrimino const *dropping_mino;
    Tetrimino const *next_minos[NEXT_AMOUNT];
    Tetrimino const *held_mino;
    bool did_already_hold;
    int dropping_mino_x, dropping_mino_y;
    int dropping_mino_spin;
    int dropping_mass_per_second;
    int lockdown_count;
    MinoSeed seed;
} Board;

void gen_board(Board *board);

Result render(Board *board, int frame, int fps);

void try_spin_left(Board *board);

void try_spin_right(Board *board);

void try_move_left(Board *board);

void try_move_right(Board *board);

void drop_softly(Board *board);

void drop_hardly(Board *board);

void try_hold(Board *board);

#endif
