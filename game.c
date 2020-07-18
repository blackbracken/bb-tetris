#include "game.h"

#include <string.h>
#include <stdlib.h>

#define LENGTH_MINO_SEED 7 * 2

const int BLOCK_AMOUNT_IN_MINO = 4;

typedef struct {
    int x, y;
} Location;

void fall_mino_once(Board *board);

bool put_and_try_next(Board *board);

bool can_move_as(const Board *board, void (*predicate)(Board *));

void spawn_mino(Board *board);

void calc_dropping_mino_locations_on_field(Board *board, Location *locations);

bool will_overlap_mass_between_fields_and_dropping_minos(Board *board);

void gen_mino_seed(MinoSeed *seed);

Tetrimino const *pop_mino(MinoSeed *seed);

void gen_shuffled_minos(Tetrimino const *shuffled[7]);

void gen_board(Board *board) {
    memset(board->field, 0, sizeof(board->field[0][0]) * FIELD_WIDTH * FIELD_HEIGHT);
    board->dropping_mass_per_second = 1;
    board->lockdown_count = 0;
    gen_mino_seed(&board->seed);

    spawn_mino(board);
}

Result render(Board *board, int frame, int fps) {
    if (!can_move_as(board, fall_mino_once)) {
        board->lockdown_count++;
    }

    if (board->lockdown_count >= fps / 2) {
        board->lockdown_count = 0;

        if (!put_and_try_next(board)) {
            Result result = {};
            result.is_available = true;
            result.score = 1000;
            result.removed_lines = 10;

            return result;
        }
    }

    if (frame % (fps / board->dropping_mass_per_second) == 0) {
        if (can_move_as(board, fall_mino_once)) {
            fall_mino_once(board);
        }
    }

    Result unavailable = {.is_available = false};
    return unavailable;
}

void move_left(Board *board) { board->dropping_mino_x--; }

void try_move_left(Board *board) {
    if (can_move_as(board, move_left)) {
        move_left(board);
        board->lockdown_count = 0;
    }
}

void move_right(Board *board) { board->dropping_mino_x++; }

void try_move_right(Board *board) {
    if (can_move_as(board, move_right)) {
        move_right(board);
        board->lockdown_count = 0;
    }
}

void drop_softly(Board *board) {
    if (can_move_as(board, fall_mino_once)) {
        fall_mino_once(board);
    }
}

void drop_hardly(Board *board) {
    while (can_move_as(board, fall_mino_once)) drop_softly(board);

    board->lockdown_count = 400;
}

void spin_right(Board *board) { board->dropping_mino_spin = (board->dropping_mino_spin + 1) % 4; }

void try_spin_right(Board *board) {
    if (can_move_as(board, spin_right)) {
        spin_right(board);
        board->lockdown_count = 0;
    }
}

void spin_left(Board *board) { board->dropping_mino_spin = (board->dropping_mino_spin + 3) % 4; }

void try_spin_left(Board *board) {
    if (can_move_as(board, spin_left)) {
        spin_left(board);
        board->lockdown_count = 0;
    }
}

void fall_mino_once(Board *board) {
    board->dropping_mino_y++;
}

bool can_move_as(const Board *board, void (*predicate)(Board *)) {
    Board assumed = *board;
    predicate(&assumed);

    Location locations[BLOCK_AMOUNT_IN_MINO];
    calc_dropping_mino_locations_on_field(&assumed, locations);
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
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


bool put_and_try_next(Board *board) {
    if (can_move_as(board, fall_mino_once)) return true;

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

    spawn_mino(board);

    // shift to upper if overlapped
    if (will_overlap_mass_between_fields_and_dropping_minos(board)) {
        board->dropping_mino_y = 1;
    }
    // it can't continue if overlapped yet
    if (will_overlap_mass_between_fields_and_dropping_minos(board)) {
        return false;
    }

    return true;
}

void spawn_mino(Board *board) {
    board->dropping_mino = pop_mino(&board->seed);
    board->dropping_mino_x = 4;
    board->dropping_mino_y = 2;
    board->dropping_mino_spin = 0;
}

void calc_dropping_mino_locations_on_field(Board *board, Location *locations) {
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
}

bool will_overlap_mass_between_fields_and_dropping_minos(Board *board) {
    Location locations[BLOCK_AMOUNT_IN_MINO];
    calc_dropping_mino_locations_on_field(board, locations);
    for (int i = 0; i < BLOCK_AMOUNT_IN_MINO; i++) {
        int x_on_field = locations[i].x;
        int y_on_field = locations[i].y;

        if (board->field[y_on_field][x_on_field] != 0) {
            return true;
        }
    }

    return false;
}

void gen_mino_seed(MinoSeed *seed) {
    seed->order = 0;
    gen_shuffled_minos(&seed->blocks[0]);
    gen_shuffled_minos(&seed->blocks[LENGTH_MINO_SEED / 2]);
}

Tetrimino const *pop_mino(MinoSeed *seed) {
    switch (seed->order) {
        case LENGTH_MINO_SEED - 1:
        case LENGTH_MINO_SEED / 2 - 1:
            gen_shuffled_minos(&seed->blocks[(seed->order + 1) % LENGTH_MINO_SEED]);
            break;
        default:
            break;
    }

    Tetrimino const *mino = seed->blocks[seed->order];

    seed->order++;
    seed->order %= LENGTH_MINO_SEED;

    return mino;
}

void gen_shuffled_minos(Tetrimino const *shuffled[7]) {
    for (int i = 0; i < 7; i++) shuffled[i] = &MINO_T;

    for (int left = 0; left < 7; left++) {
        int right = rand() % 7;
        Tetrimino const *temp = shuffled[right];
        shuffled[right] = shuffled[left];
        shuffled[left] = temp;
    }
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