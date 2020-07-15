#include "game.h"

const int BLOCK_AMOUNT_IN_MINO = 4;

typedef struct {
    int x, y;
} Location;

int calc_dropping_mino_locations_on_field(Board *board, Location *locations);

bool will_overlap_mass_between_fields_and_dropping_minos(Board *board);

bool can_move(const Board *board, void (*predicate)(Board *)) {
    Board assumed = *board;
    predicate(&assumed);

    Location locations[BLOCK_AMOUNT_IN_MINO];
    // TODO: change amount to 4 because it must be 4.
    int mino_block_amount = calc_dropping_mino_locations_on_field(&assumed, locations);
    for (int i = 0; i < mino_block_amount; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (x_on_field < 0 || y_on_field < 0
            || FIELD_WIDTH <= x_on_field || FIELD_HEIGHT <= y_on_field
            || assumed.field[y_on_field][x_on_field] != 0) {
            return false;
        }
    }

    return true;
}

void move_left(Board *board) { board->dropping_mino_x--; }

void move_right(Board *board) { board->dropping_mino_x++; }

void drop_softly(Board *board) { board->dropping_mino_y++; }

void drop_hardly(Board *board) {
    while (can_move(board, drop_softly)) drop_softly(board);
    put_and_try_next(board);
}

void spin_right(Board *board) { board->dropping_mino_spin = (board->dropping_mino_spin + 1) % 4; }

void spin_left(Board *board) { board->dropping_mino_spin = (board->dropping_mino_spin + 3) % 4; }

bool put_and_try_next(Board *board) {
    if (can_move(board, drop_softly)) return true;

    Location locations[BLOCK_AMOUNT_IN_MINO];

    calc_dropping_mino_locations_on_field(board, locations);
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (y_on_field < FIELD_HEIGHT) {
            board->field[y_on_field][x_on_field] = 1;
        } else {
            return false;
        }
    }

    // spawn new mino
    board->dropping_mino = &MINO_T; // TODO: pick randomly
    board->dropping_mino_x = 4;
    board->dropping_mino_y = 1;
    board->dropping_mino_spin = 0;

    if (will_overlap_mass_between_fields_and_dropping_minos(board)) {
        board->dropping_mino_y = 0;
    }
    if (will_overlap_mass_between_fields_and_dropping_minos(board)) {
        return false;
    }

    return true;
}

int calc_dropping_mino_locations_on_field(Board *board, Location *locations) {
    int amount = 0;

    for (int j = 0; j < board->dropping_mino->size; j++) {
        for (int i = 0; i < board->dropping_mino->size; i++) {
            if (board->dropping_mino->shape[board->dropping_mino_spin][j][i] == 0) {
                continue;
            }

            Location loc_on_field = {
                    .x = board->dropping_mino_x + (i - board->dropping_mino->center_x),
                    .y = board->dropping_mino_y + (j - board->dropping_mino->center_y),
            };
            locations[amount++] = loc_on_field;
        }
    }

    return amount;
}

bool will_overlap_mass_between_fields_and_dropping_minos(Board *board) {
    Location locations[BLOCK_AMOUNT_IN_MINO];
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (board->field[y_on_field][x_on_field] != 0) {
            return true;
        }
    }

    return false;
}

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