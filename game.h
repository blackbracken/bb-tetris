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
    MinoBlock field[FIELD_HEIGHT][FIELD_WIDTH];
    Tetrimino const *dropping_mino;
    int dropping_mino_x, dropping_mino_y;
    int dropping_mino_spin;
    int dropping_mass_per_second;
} Board;

// TODO: rename to better
bool can_move(const Board *board, void (*predicate)(Board *));

void move_left(Board *board);

void move_right(Board *board);

void drop_softly(Board *board);

void drop_hardly(Board *board);

void spin_left(Board *board);

void spin_right(Board *board);

bool put_and_try_next(Board *board);

#endif
