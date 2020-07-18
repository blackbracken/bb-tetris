#ifndef BBTETRIS_GAME_H
#define BBTETRIS_GAME_H

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 21

#include <stdbool.h>

typedef enum {
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    AIR
} MinoBlock;

typedef struct {
    const int center_x, center_y;
    const int shape[4][4][4];
    const int size;
    const int spin_offsets[4][5][2];
} Tetrimino;

extern const Tetrimino MINO_T;

typedef struct {
    int score;
    int removed_lines;
} Result;

typedef struct {
    int frame;
    MinoBlock field[FIELD_HEIGHT][FIELD_WIDTH];
    Tetrimino const *dropping_mino;
    int dropping_mino_x, dropping_mino_y;
    int dropping_mino_spin;
    int dropping_mass_per_second;
    int lockdown_count;
    Result *result;
} Board;

void gen_board(Board *board);

Result *render(Board *board, int frame, int fps);

void try_spin_left(Board *board);

void try_spin_right(Board *board);

void try_move_left(Board *board);

void try_move_right(Board *board);

void drop_softly(Board *board);

void drop_hardly(Board *board);

#endif
