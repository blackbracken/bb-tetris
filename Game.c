#include "Game.h"

int can_move(Board *board, void (*predicate)(Board *)) {
    Board assumed = *board;
    predicate(&assumed);

    for (int j = 0; j < assumed.dropping_mino->size; j++) {
        for (int i = 0; i < assumed.dropping_mino->size; i++) {
            if (assumed.dropping_mino->shape[assumed.dropping_mino_spin][j][i] == 0) {
                continue;
            }
            int dest_x = assumed.dropping_mino_x + (i - assumed.dropping_mino->center_x);
            int dest_y = assumed.dropping_mino_y + (j - assumed.dropping_mino->center_y);

            if (dest_x < 0 || dest_y < 0
                || FIELD_WIDTH <= dest_x || FIELD_HEIGHT <= dest_y
                || assumed.field[dest_y][dest_x] != 0) {
                return 0;
            }
        }
    }

    return 1;
}

void move_left(Board *board) { board->dropping_mino_x--; }

void move_right(Board *board) { board->dropping_mino_x++; }

void drop_softly(Board *board) { board->dropping_mino_y++; }

void drop_hardly(Board *board) { while (can_move(board, drop_softly)) drop_softly(board); }

void spin_right(Board *board) { board->dropping_mino_spin = (board->dropping_mino_spin + 1) % 4; }

void spin_left(Board *board) { board->dropping_mino_spin = (board->dropping_mino_spin + 3) % 4; }

// TODO: write spinning offsets
const Tetrimino MINO_T = {
        .center_x =  1,
        .center_y =  1,
        .shape = {
                {
                        {0, 1, 0},
                        {1, 1, 1},
                        {0, 0, 0},
                },
                {
                        {0, 1, 0},
                        {0, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 0, 0},
                        {1, 1, 1},
                        {0, 1, 0},
                },
                {
                        {0, 1, 0},
                        {1, 1, 0},
                        {0, 1, 0},
                },
        },
        .size = 3,
};